# GRILS execution
import subprocess

# File import
from os import listdir
from os.path import isfile, join



pathToCordeau = "./MCTOPMTW-Cordeau/"
pathToSolomon = "./MCTOPMTW-Solomon/"
CordeauFiles = [pathToCordeau + f for f in listdir(pathToCordeau) if isfile(join(pathToCordeau, f))]
SolomonFiles = [pathToSolomon + f for f in listdir(pathToSolomon) if isfile(join(pathToSolomon, f))]

PATHS = CordeauFiles + SolomonFiles
#for i in PATHS:
    #print (i)




def EX_GRILS (pathToGrils, TestFile):
    args = ("time", pathToGrils , "-t" , TestFile)
    popen = subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    popen.wait()
    output = popen.stdout.read().decode()
    errput = popen.stderr.read().decode()
    #print (output)
    #print (errput)

    tab = [x.split() for x in output.split('\n')] + [x.split() for x in errput.split('\n')]
    
    isValid = True
    Result = 0
    time = 0
    
    for string in tab :
        if (len (string) > 3 and string[3] == "Solution" ): isValid = string[1] == "OK" and isValid
        if (len (string) > 5 and string[1] == "RESULT") : Result = int(string[5])
        if (len (string) > 2 and string[1] == "real") : time = float(string[0])

    #print (isValid, Result, time)
    return (isValid, Result, time)


def testFile(pathToGrils, filePath, Numberit, maxFailit):

    filename = filePath[19:]
    
    totalScore = 0
    worstScore = float("inf")
    bestScore = float("-inf")
    totalTime = 0
    i, totali= 0,0
    str_result = ""
    
    while (i < Numberit and maxFailit > 0):
        isValid, Result, time = EX_GRILS(pathToGrils,filePath)
        if (isValid) :
            i+=1
            totalScore += Result
            totalTime += time
            worstScore = min(worstScore, Result)
            bestScore = max(bestScore,Result)
        else : maxFailit-=1
    
    if (maxFailit == 0):
        print("ERROR with file " + filename)
    else :
        str_result =  filename + ","
        str_result += str(round(bestScore,1)) + ","
        str_result += str(round(totalScore/Numberit,1)) + ","
        str_result += str(round(worstScore,1)) + ","
        str_result += str(round(totalTime/Numberit,1))
        str_result += "\n"

    return str_result



# CSV export
pathToGrils = "../GRILS_UNIX/grils"
file = open("Results.csv", 'a')
file.write("file name,benchmark,best,average,worst,time\n")
file.close()

for i in range (len(PATHS)):
    
    str_result = testFile(pathToGrils, PATHS[i], 10, 1)
    if (len(str_result) > 0):
        file = open("Results.csv", 'a')
        file.write(str_result)
        file.close()
        print ("done : " + str(i+1) + "/" + str(len(PATHS)) + " - result is : str_result")
    else :
        file = open("Results.csv", 'a')
        print ("fail : " + str(i+1) + "/" + str(len(PATHS)))
        file.close()

        
file.close()
