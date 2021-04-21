#!pip install wget
import requests
import wget
from bs4 import BeautifulSoup as bs

# set web directory URL
url = 'http://ranger.uta.edu/~weems/NOTES5311/LAB/LAB3SPR21/'
# exts: download files with these extensions
exts = {'c', 'dat', 'out', 'pdf'}
# dest: save directory for downloaded files
dest ='./'

def listFD(url, exts=list):
  # get page content in form of text
  page = requests.get(url).text
  items = list()
  soup = bs(page, 'html.parser')
  for node in soup.find_all('a'):
    for e in exts:
      if (node.get('href').endswith(e)):
        items.append(node.get('href'))
  return items


for file in listFD(url, exts):
  print(file, '   saved to:', dest)
  wget.download(url+file)


# EOF
