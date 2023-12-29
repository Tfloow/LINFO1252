import subprocess

name = lambda x: "cours" + x 
subprocess.run(["ls"])

for i in range(1, 13):
    file = name(str(i))
    subprocess.run(["pandoc.exe" , f"{file}.md", "-o", f".\\TeX_Summary\\{file}.tex"])