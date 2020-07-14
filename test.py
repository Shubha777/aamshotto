# opening file
import time

f = open("C:/test/aamshotto.txt")
name = f.readline()
print(name)
name = name.replace(" ", "+")
import requests
from googlesearch import search

query = "AniList.co manga " + name
print(query)
mangalinklist = []
for j in search(query, tld="com", num=5, stop=4, pause=0.5):
    mangalinklist.append(j)

    print(mangalinklist)

for jojo in mangalinklist:
    if "manga" in jojo:
        if "anilist.co" in jojo:
            print(jojo)
            mangalink = jojo
            break

mangaID = ""

print("mangalink", mangalink)
for jo in mangalink:
    if jo.isdigit():
        mangaID += jo
print(mangaID)

query = '''
query ($id: Int) { # Define which variables will be used in the query (id)
  Media (id: $id, type: MANGA) { # Insert our variables into the query arguments (id) (type: ANIME is hard-coded in the query)
    id
    title {
      romaji
      english
      native
    }
    coverImage
    {
     extraLarge
    }
    
  }
}
'''
variables = {
    'id': mangaID
}
url = 'https://graphql.anilist.co'
response = requests.post(url, json={'query': query, 'variables': variables})
ans = response.json()
imglink = ans['data']['Media']['coverImage']['extraLarge']
print(imglink)
with open('c:/test/mangapic.jpg', 'wb') as handle:
    response = requests.get(imglink, stream=True)

    for block in response.iter_content(1024):
        if not block:
            break

        handle.write(block)

from PIL import Image

Image.open("c:/test/mangapic.jpg").save("c:/test/sample1.bmp")
