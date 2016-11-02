import os
from sys import argv

_, zipfile = argv

os.system('unzip ' + zipfile)
fail = False

if not os.path.exists(zipfile[:-4] + '/compile.sh'):
	fail = True
	print('missing compile.sh')

if not os.path.exists(zipfile[:-4] + '/run.sh'):
	fail = True
	print('missing run.sh')

if not os.path.exists(zipfile[:-4] + '/writeup.txt'):
	fail = True
	print('missing writeup.txt')

if not os.path.exists(zipfile[:-4] + '/compile_bonus.sh'):	
	print('missing compile_bonus.sh')

if not os.path.exists(zipfile[:-4] + '/run_bonus.sh'):	
	print('missing run_bonus.sh')

if not os.path.exists(zipfile[:-4] + '/writeup.pdf'):	
	print('missing writeup.pdf')

if fail: 
	print('========================\nNOT READY FOR SUBMISSION\n========================')
else:
	print('======\nPASSED\n======')
