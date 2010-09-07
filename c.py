bltin=[';','.','drop']

from sys import stdin

prog={}
wid={'main':0}

def makewid(w):
	if w not in wid:
		wid[w]=max(wid.values())+1
	return wid[w]

for x in stdin:
	w=x.split()
	df=[]
	for x in w[1:]:
		if x in bltin:
			df.append("JZ|JN|JP|CALL|BLTIN|%u"%bltin.index(x))
		else:
			df.append("JZ|JN|JP|CALL|%u"%makewid(x))
	prog[w[0]]=df
	makewid(w[0])

print '#include <stdint.h>'
print '#include "prefix.h"'
for x,y in prog.items():
	print 'static uint16_t prog_%u[]={%s|JMP};'%(wid[x],','.join(y))

print 'uint16_t *prog[]={%s};'%(','.join(['prog_%s'%x for x in range(0,len(wid))]))

