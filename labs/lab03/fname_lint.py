
import os

srcDir = './'

def main():

  for i, file in enumerate(os.listdir(srcDir)):
    #print(i, file)
    newfile = file
    newfile = newfile.replace('lab3spr21.', 'test_')
    newfile = newfile.replace('notes14.p14', 'test_notes_')
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
