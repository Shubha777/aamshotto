import requests
import os
from bs4 import BeautifulSoup
import json
import lxml
search="http://www.mangareader.net/search"
f = open("C:/test/aamshotto.txt")
name = f.readline()
name = name.replace(" ", "+")
search_url=search+"/?w="+name+"&rd=0&status=0&order=0&genre=&p=0"
r = requests.get(search_url)
print(search_url)
soup = BeautifulSoup(r.content, 'lxml')
#print(soup.prettify())
linklist=soup.select(".manga_name")

linkset=[]
for linkname in linklist:

    html=str(linkname)
    resoup=BeautifulSoup(html,'lxml')
    link= resoup.find("a").get('href')
    linkset.append(link)


linkpre="http://www.mangareader.net"

LINKS=[]
for j in linkset:
    link=linkpre+j
    LINKS.append(link)


chf = open("C:/test/chapno.txt")
chno = chf.readline()
chno=int(chno)
resno=0
for url in LINKS:
    churl=url+"/"+str(chno)
    imgfolderlis=[]
    pageno=1
    while True:
       try:
            chpage=churl+"/"+str(pageno)


            req = requests.get(chpage)
            soup = BeautifulSoup(req.content, 'lxml')
            html = soup.find("div",id="imgholder")
            imglink=html.find("img").get("src")
            imgfolderlis.append(imglink)
            #print(imglink)
            pageno+=1
       except:
           break
    #print(imgfolderlis,'imgfolder',len(imgfolderlis))
    try:
      for j in range(0,len(imgfolderlis)):
        manganame=linkset[resno].replace("-","")
        filelocation="c:/manga/"+manganame+"/"+str(chno)+"/"+str(j+1)+".jpg"
        os.makedirs(os.path.dirname(filelocation), exist_ok=True)
        print(filelocation)
        with open(filelocation, 'wb') as handle:
            print(imgfolderlis[j])
            response = requests.get(imgfolderlis[j], stream=True)


            for block in response.iter_content(1024):
                if not block:
                    break

                handle.write(block)
    except:
         break
    resno += 1
    break

