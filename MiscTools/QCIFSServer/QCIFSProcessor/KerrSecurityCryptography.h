//*****************************************************************************
//
//  Author:         Kenny Kerr
//  Date created:   24 February 2007
//
//  Description:    Provides a set of light wrapper classes around the core 
//                  BCrypt objects from the Cryptography Next Generation API.
//
//*****************************************************************************

#pragma once
void PrintHexDump(DWORD length, PBYTE buffer);

namespace Kerr { namespace Security { namespace Cryptography
{
    class BObject
    {
    public:

        BCRYPT_HANDLE GetHandle() const
        {
            return m_handle;
        }

        __checkReturn
        NTSTATUS GetProperty(PCWSTR name,
                             __out_bcount_part_opt(outputSize, result) void* output,
                             ULONG outputSize,
                             __out ULONG& result,
                             ULONG flags) const
        {
            ASSERT(0 != m_handle);
            ASSERT(0 != name);

            return ::BCryptGetProperty(m_handle,
                                       name,
                                       static_cast<PUCHAR>(output),
                                       outputSize,
                                       &result,
                                       flags);
        }

        template <typename T>
        __checkReturn
        NTSTATUS GetProperty(PCWSTR name,
                             __out T& output,
                             ULONG flags) const
        {
            ASSERT(0 != m_handle);
            ASSERT(0 != name);

            ULONG bytesCopied = 0;

            NTSTATUS status = GetProperty(name,
                                          &output,
                                          sizeof(T),
                                          bytesCopied,
                                          flags);

            ASSERT((NT_SUCCESS(status) && sizeof(T) == bytesCopied) || (!NT_SUCCESS(status) && 0 == bytesCopied));
            return status;
        }

        __checkReturn
        NTSTATUS SetProperty(PCWSTR name,
                             __in_bcount(valueSize) const void* value,
                             ULONG valueSize,
                             ULONG flags)
        {
            ASSERT(0 != m_handle);
            ASSERT(0 != name);

            return ::BCryptSetProperty(m_handle,
                                       name,
                                       static_cast<PUCHAR>(const_cast<void*>(value)),
                                       valueSize,
                                       flags);
        }

        template <typename T>
        __checkReturn
        NTSTATUS SetProperty(PCWSTR name,
                             const T& value,
                             ULONG flags)
        {
            ASSERT(0 != m_handle);
            ASSERT(0 != name);

            return SetProperty(name,
                               &value,
                               sizeof(T),
                               flags);
        }

    protected:

        BObject() :
            m_handle(0)
        {
            // Do nothing
        }

        ~BObject()
        {
            // Derived classes must remember to release the handle.
            ASSERT(0 == m_handle);
        }

        BCRYPT_HANDLE m_handle;
    };

    class Buffer
    {
    public:

        Buffer() :
            m_ptr(0),
            m_size(0)
        {
            // Do nothing
        }

        ~Buffer()
        {
            Destroy();
        }

        __checkReturn
        NTSTATUS Create(ULONG size)
        {
            ASSERT(0 == m_ptr);
            ASSERT(0 == m_size);

            m_ptr = new UCHAR[size];

            NTSTATUS status = 0 != m_ptr ? STATUS_SUCCESS : STATUS_INSUFFICIENT_RESOURCES;

            if (NT_SUCCESS(status))
            {
                m_size = size;

                ::RtlZeroMemory(m_ptr,
                                size);
            }

            return status;
        }

        void Destroy()
        {
            if (0 != m_ptr)
            {
                ::SecureZeroMemory(m_ptr,
                                   m_size);

                delete [] m_ptr;
                m_ptr = 0;
                m_size = 0;
            }
        }

        PUCHAR GetData()
        {
            return m_ptr;
        }

        ULONG GetSize() const
        {
            return m_size;
        }

        void print()
        {
          if(m_size && m_ptr)
            PrintHexDump(m_size, m_ptr);
        }

    public:

        PUCHAR m_ptr;
        ULONG m_size;

    };

    class AlgorithmProvider : public BObject
    {
    public:

        AlgorithmProvider()
        {
            // Do nothing
        }

        ~AlgorithmProvider()
        {
            NTSTATUS status = Close(0);
            ASSERT(NT_SUCCESS(status));
            status;
        }

        __checkReturn
        NTSTATUS Open(PCWSTR algorithmIdentifier,
                      __in_opt PCWSTR providerImplementation,
                      ULONG flags)
        {
            ASSERT(0 == m_handle);
            ASSERT(0 != algorithmIdentifier);

            return ::BCryptOpenAlgorithmProvider(&m_handle,
                                                 algorithmIdentifier,
                                                 providerImplementation,
                                                 flags);
        }

        __checkReturn
        NTSTATUS Close(ULONG flags)
        {
            NTSTATUS status = STATUS_SUCCESS;
            
            if (0 != m_handle)
            {
                status = ::BCryptCloseAlgorithmProvider(m_handle,
                                                        flags);
                m_handle = 0;
            }

            return status;
        }

        __checkReturn
        NTSTATUS GenRandom(__inout_bcount_full(bufferSize) void* buffer,
                           ULONG bufferSize,
                           ULONG flags)
        {
            ASSERT(0 != m_handle);
            ASSERT(0 != buffer);

            return ::BCryptGenRandom(m_handle,
                                     static_cast<PUCHAR>(buffer),
                                     bufferSize,
                                     flags);
        }

        template <typename T>
        __checkReturn
        NTSTATUS GenRandom(__inout T& buffer,
                           ULONG flags)
        {
            return GenRandom(&buffer,
                             sizeof(T),
                             flags);
        }

    };

    class Hash : public BObject
    {
    public:

        Hash()
        {
            // Do nothing
        }

        ~Hash()
        {
            NTSTATUS status = Destroy();
            ASSERT(NT_SUCCESS(status));
            status;
        }

        __checkReturn
        NTSTATUS Create(const AlgorithmProvider& algorithm,
                        __in_bcount_opt(secretSize) const void* secret,
                        ULONG secretSize,
                        ULONG flags)
        {
            ASSERT(0 == m_handle);
            ASSERT(0 == m_buffer.GetSize());

            ULONG bufferSize = 0;

            NTSTATUS status = algorithm.GetProperty(BCRYPT_OBJECT_LENGTH,
                                                    bufferSize,
                                                    0); // flags

            if (NT_SUCCESS(status))
            {
                status = m_buffer.Create(bufferSize);

                if (NT_SUCCESS(status))
                {
                    ASSERT(0 < m_buffer.GetSize());

                    status = ::BCryptCreateHash(algorithm.GetHandle(),
                                                &m_handle,
                                                m_buffer.GetData(),
                                                m_buffer.GetSize(),
                                                static_cast<PUCHAR>(const_cast<void*>(secret)),
                                                secretSize,
                                                flags);

                    if (!NT_SUCCESS(status))
                    {
                        m_buffer.Destroy();
                    }
                }
            }

            return status;
        }

        __checkReturn
        NTSTATUS DuplicateHash(const Hash& hash,
                               ULONG flags)
        {
            ASSERT(0 == m_handle);
            ASSERT(0 == m_buffer.GetSize());
            ASSERT(0 != hash.m_handle);

            NTSTATUS status = m_buffer.Create(hash.m_buffer.GetSize());

            if (NT_SUCCESS(status))
            {
                ASSERT(0 < m_buffer.GetSize());

                status = ::BCryptDuplicateHash(hash.GetHandle(),
                                               &m_handle,
                                               m_buffer.GetData(),
                                               m_buffer.GetSize(),
                                               flags);

                if (!NT_SUCCESS(status))
                {
                    m_buffer.Destroy();
                }
            }

            return status;
        }

        __checkReturn
        NTSTATUS Destroy()
        {
            NTSTATUS status = STATUS_SUCCESS;

            if (0 != m_handle)
            {
                status =  ::BCryptDestroyHash(m_handle);
                m_handle = 0;
                m_buffer.Destroy();
            }

            ASSERT(0 == m_buffer.GetSize());
            return status;
        }

        __checkReturn
        NTSTATUS HashData(__in_bcount(inputSize) const void* input,
                          ULONG inputSize,
                          ULONG flags)
        {
            ASSERT(0 != m_handle);

            return ::BCryptHashData(m_handle,
                                    static_cast<PUCHAR>(const_cast<void*>(input)),
                                    inputSize,
                                    flags);
        }

        template <typename T>
        __checkReturn
        NTSTATUS HashData(const T& input,
                          ULONG flags)
        {
            return HashData(&input,
                            sizeof(T),
                            flags);
        }

        __checkReturn
        NTSTATUS FinishHash(__out_bcount_full(outputSize) void* output,
                            ULONG outputSize,
                            ULONG flags)
        {
            ASSERT(0 != output);

            return ::BCryptFinishHash(m_handle,
                                      static_cast<PUCHAR>(output),
                                      outputSize,
                                      flags);
        }

        __checkReturn
        NTSTATUS GetHashSize(__out ULONG& value) const
        {
            ASSERT(0 != m_handle);

            return GetProperty(BCRYPT_HASH_LENGTH,
                               value,
                               0);
        }

    private:

        Buffer m_buffer;

    };

    class Key : public BObject
    {
    public:

        Key()
        {
            // Do nothing
        }

        ~Key()
        {
            NTSTATUS status = Destroy();
            ASSERT(NT_SUCCESS(status));
            status;
        }

        __checkReturn
        NTSTATUS GenerateKeyPair(const AlgorithmProvider& algorithm,
                                 ULONG keyBitSize,
                                 ULONG flags)
        {
            ASSERT(0 == m_handle);

            return ::BCryptGenerateKeyPair(algorithm.GetHandle(),
                                           &m_handle,
                                           keyBitSize,
                                           flags);
        }

        __checkReturn
        NTSTATUS FinalizeKeyPair(ULONG flags)
        {
            ASSERT(0 != m_handle);

            return ::BCryptFinalizeKeyPair(m_handle,
                                           flags);
        }

        __checkReturn
        NTSTATUS GenerateSymmetricKey(const AlgorithmProvider& algorithm,
                                      __in_bcount(secretSize) const void* secret,
                                      ULONG secretSize,
                                      ULONG flags)
        {
            ASSERT(0 == m_handle);
            ASSERT(0 == m_buffer.GetSize());

            ULONG bufferSize = 0;

            NTSTATUS status = algorithm.GetProperty(BCRYPT_OBJECT_LENGTH,
                                                    bufferSize,
                                                    0); // flags

            if (NT_SUCCESS(status))
            {
                status = m_buffer.Create(bufferSize);

                if (NT_SUCCESS(status))
                {
                    ASSERT(0 < m_buffer.GetSize());

                    status = ::BCryptGenerateSymmetricKey(algorithm.GetHandle(),
                                                          &m_handle,
                                                          m_buffer.GetData(),
                                                          m_buffer.GetSize(),
                                                          static_cast<PUCHAR>(const_cast<void*>(secret)),
                                                          secretSize,
                                                          flags);

                    if (!NT_SUCCESS(status))
                    {
                        m_buffer.Destroy();
                    }
                }
            }

            return status;
        }

        __checkReturn
        NTSTATUS ImportKey(const AlgorithmProvider& algorithm,
                           __in_opt BCRYPT_KEY_HANDLE reserved,
                           PCWSTR blobType,
                           __in_bcount(inputSize) const void* input,
                           ULONG inputSize,
                           ULONG flags)
        {
            ASSERT(0 == m_handle);
            ASSERT(0 == m_buffer.GetSize());
            ASSERT(0 != blobType);
            ASSERT(0 != input);

            ULONG bufferSize = 0;

            NTSTATUS status = algorithm.GetProperty(BCRYPT_OBJECT_LENGTH,
                                                    bufferSize,
                                                    0); // flags

            if (NT_SUCCESS(status))
            {
                status = m_buffer.Create(bufferSize);

                if (NT_SUCCESS(status))
                {
                    ASSERT(0 < m_buffer.GetSize());

                    status = ::BCryptImportKey(algorithm.GetHandle(),
                                               reserved,
                                               blobType,
                                               &m_handle,
                                               m_buffer.GetData(),
                                               m_buffer.GetSize(),
                                               static_cast<PUCHAR>(const_cast<void*>(input)),
                                               inputSize,
                                               flags);


                    if (!NT_SUCCESS(status))
                    {
                        m_buffer.Destroy();
                    }
                }
            }

            return status;
        }

        __checkReturn
        NTSTATUS ImportKeyPair(const AlgorithmProvider& algorithm,
                               __in_opt BCRYPT_KEY_HANDLE reserved,
                               PCWSTR blobType,
                               __in_bcount(inputSize) const void* input,
                               ULONG inputSize,
                               ULONG flags)
        {
            ASSERT(0 == m_handle);
            ASSERT(0 == m_buffer.GetSize());
            ASSERT(0 != blobType);
            ASSERT(0 != input);

            return ::BCryptImportKeyPair(algorithm.GetHandle(),
                                         reserved,
                                         blobType,
                                         &m_handle,
                                         static_cast<PUCHAR>(const_cast<void*>(input)),
                                         inputSize,
                                         flags);
        }

        __checkReturn
        NTSTATUS Destroy()
        {
            NTSTATUS status = STATUS_SUCCESS;

            if (0 != m_handle)
            {
                status =  ::BCryptDestroyKey(m_handle);
                m_handle = 0;
                m_buffer.Destroy();
            }

            ASSERT(0 == m_buffer.GetSize());
            return status;
        }

        __checkReturn
        NTSTATUS Encrypt(__in_bcount(plaintextSize) const void* plaintext,
                         ULONG plaintextSize,
                         __in_opt const void* paddingInfo,
                         __inout_bcount_opt(initializationVectorSize) void* initializationVector,
                         ULONG initializationVectorSize,
                         __out_bcount_part_opt(ciphertextSize, resultSize) void* ciphertext,
                         ULONG ciphertextSize,
                         ULONG& resultSize,
                         ULONG flags)
        {
            ASSERT(0 != m_handle);
            ASSERT(0 != plaintext);

            return ::BCryptEncrypt(m_handle,
                                   static_cast<PUCHAR>(const_cast<void*>(plaintext)),
                                   plaintextSize,
                                   const_cast<void*>(paddingInfo),
                                   static_cast<PUCHAR>(initializationVector),
                                   initializationVectorSize,
                                   static_cast<PUCHAR>(ciphertext),
                                   ciphertextSize,
                                   &resultSize,
                                   flags);
        }

        __checkReturn
        NTSTATUS Decrypt(__in_bcount(ciphertextSize) const void* ciphertext,
                         ULONG ciphertextSize,
                         __in_opt const void* paddingInfo,
                         __inout_bcount_opt(initializationVectorSize) void* initializationVector,
                         ULONG initializationVectorSize,
                         __out_bcount_part_opt(plaintextSize, resultSize) void* plaintext,
                         ULONG plaintextSize,
                         ULONG& resultSize,
                         ULONG flags)
        {
            ASSERT(0 != m_handle);

            return ::BCryptDecrypt(m_handle,
                                   static_cast<PUCHAR>(const_cast<void*>(ciphertext)),
                                   ciphertextSize,
                                   const_cast<void*>(paddingInfo),
                                   static_cast<PUCHAR>(initializationVector),
                                   initializationVectorSize,
                                   static_cast<PUCHAR>(plaintext),
                                   plaintextSize,
                                   &resultSize,
                                   flags);
        }

        __checkReturn
        NTSTATUS SignHash(__in_opt const void* paddingInfo,
                          __in_bcount(inputSize) const void* input,
                          ULONG inputSize,
                          __out_bcount_part_opt(outputSize, resultSize) void* output,
                          ULONG outputSize,
                          ULONG& resultSize,
                          ULONG flags)
        {
            ASSERT(0 != m_handle);

            return ::BCryptSignHash(m_handle,
                                    const_cast<void*>(paddingInfo),
                                    static_cast<PUCHAR>(const_cast<void*>(input)),
                                    inputSize,
                                    static_cast<PUCHAR>(output),
                                    outputSize,
                                    &resultSize,
                                    flags);
        }

        __checkReturn
        NTSTATUS VerifySignature(__in_opt const void* paddingInfo,
                                 __in_bcount(hashSize) const void* hash,
                                 ULONG hashSize,
                                 __in_bcount(signatureSize) const void* signature,
                                 ULONG signatureSize,
                                 ULONG flags)
        {
            ASSERT(0 != m_handle);

            return ::BCryptVerifySignature(m_handle,
                                           const_cast<void*>(paddingInfo),
                                           static_cast<PUCHAR>(const_cast<void*>(hash)),
                                           hashSize,
                                           static_cast<PUCHAR>(const_cast<void*>(signature)),
                                           signatureSize,
                                           flags);
        }

        __checkReturn
        NTSTATUS ExportKey(__in_opt BCRYPT_KEY_HANDLE reserved,
                           PCWSTR blobType,
                           __out_bcount_part_opt(outputSize, resultSize) void* output,
                           ULONG outputSize,
                           ULONG& resultSize,
                           ULONG flags)
        {
            ASSERT(0 != m_handle);
            ASSERT(blobType);

            return ::BCryptExportKey(m_handle,
                                     reserved,
                                     blobType,
                                     static_cast<PUCHAR>(output),
                                     outputSize,
                                     &resultSize,
                                     flags);

        }

    private:

        Buffer m_buffer;

    };

}}}
