
import os

srcDir = './'

def main():

  for i, file in enumerate(os.listdir(srcDir)):
    #print(i, file)
    newfile = file
    newfile = newfile.replace('.dat', '.txt')
    src = srcDir+file
    dest = srcDir+newfile
    os.rename(src,dest)
    #print(i, file)

  print('files renamed... ')
  for i, file in enumerate(os.listdir(srcDir)):
    print(i, file)


if __name__ == '__main__':

  main()


# EOF
