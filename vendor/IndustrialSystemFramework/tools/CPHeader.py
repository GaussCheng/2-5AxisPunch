import sys
import os
import shutil

if len(sys.argv) < 3:
    print("Usage python CPHeader inputDir outputDir")

inputPath = os.path.expanduser(sys.argv[1])
outputPath = os.path.expanduser(sys.argv[2])
if not os.path.exists(inputPath):
    print("no such dir " + inputPath)
    exit(0)

if os.path.exists(outputPath):
    shutil.rmtree(outputPath)

os.mkdir(outputPath)
for root, dirs, files in os.walk(inputPath):
    if os.path.abspath(root) == os.path.abspath(outputPath):
        continue
    for file in files:
        if file[-2 : len(file)] == ".h":
#            print file
            shutil.copyfile(os.path.join(root, file), os.path.join(outputPath, file))
#            print os.path.join(root, file)
