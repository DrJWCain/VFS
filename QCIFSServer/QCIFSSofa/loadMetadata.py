# Copyright 2018 Grass Valley, A Belden Brand

# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at

#     http://www.apache.org/licenses/LICENSE-2.0

# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import glob, os, json, requests
import eyed3, eyed3.mp3

def index(pathToSearch, format):
    # print(pathToSearch)
    search = os.path.join(pathToSearch, format)
    # print(search)
    files = glob.glob(search)
    # print(files)
    files.sort()
    names = [os.path.basename(f) for f in files]
    # print(names)
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

        filePath = path+name
        # f = open(filePath, "r")

        # print(dir(eyed3.core))

        print( eyed3.core.guessMimetype(filePath))
        if eyed3.mp3.isMp3File(filePath):
            audioFile = eyed3.mp3.Mp3AudioFile(filePath)
            info = audioFile.info
            record["Duration"] = info.time_secs
            # print(dir(info))
            # print(info.lame_tag)
            # print(dir(info.mp3_header))
            # print(info.mp3_header.copyright)
            tags = audioFile.tag
            # print(dir(tags))
            # print(tags.artist)
            # print(tags.track_num[0])
            # print(tags.title)
            # print(tags.lyrics)
            record["TrackNum"] = tags.track_num[0]

        print(str(record))
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

folder("Motorhead", "D:\\Music\\M\\Motorhead - The Best Of Greatest Hits\\")
masterFolder("Bowie", "D:\\Music\\B\\Bowie 1966 - 1976\\*")
# masterFolder("Hawkwind", "D:\\Music\\H\\Hawkwind\\*")
masterFolder("Led Zeppelin", "D:\\Music\\L\\Led Zeppelin\\*")

