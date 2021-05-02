
import os
import subprocess

dataDir = './data/'
outDir = './out/'

def main():

  for i, file in enumerate(os.listdir(dataDir)):
    print(i, file)
    newfile = file
    newfile = newfile.replace('.txt', '_01.txt')
    newfile = newfile.replace('test', 'nbug_t')
    #src = srcDir+file
    #dest = srcDir+newfile
    #os.rename(src,dest)
    #print(i, file)
    #output = subprocess.run(['rm', outDir+file])
    #print(output)
    output = subprocess.run(['./lcp.out', ' < '+dataDir+file]) #+' > '+outDir+newfile])
    print(i+'  '+outDir+newfile+': '+output)




if __name__ == '__main__':

  main()


# EOF
