import glob, os, json, requests

def index(pathToSearch, format):
    files = glob.glob(os.path.join(pathToSearch, format))
    files.sort()
    names = [os.path.basename(f) for f in files]
    print(str(names))
    return names



url = "http://127.0.0.1:5984/music/"
artist = "Bowie"
path = "D:\\Music\\B\\Bowie 1966 - 1976\\David Bowie - Hunky Dory\\"
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

    responce = requests.post(url, json=record)
    print(str(responce))


