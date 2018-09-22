import glob, os, json, requests

def index(pathToSearch, format):
    files = glob.glob(os.path.join(pathToSearch, format))
    files.sort()
    names = [os.path.basename(f) for f in files]
    print(str(names))
    return names



url = "http://127.0.0.1:5984/music/"

def folder(artist, path):
    names = index(path, "*.mp3")

    for name in names:
        record = {
            "Type": "Recording",
            "Artist": artist,
            'Name': name,
            'Path': path,
        }
        # print(str(record))
        # val = json.dumps(record)
        # print(val)

        reply = requests.post(url, json=record)
        # print(str(reply.status_code))



def masterFolder(artist, path):
    files = glob.glob(path)
    files.sort()
    # names = [os.path.basename(f) for f in files]
    names = files
    print(str(names))

    for name in names:
        folder(artist, name + "\\")

masterFolder("Bowie", "D:\\Music\\B\\Bowie 1966 - 1976\\*")

masterFolder("Hawkwind", "D:\\Music\\H\\Hawkwind\\*")
