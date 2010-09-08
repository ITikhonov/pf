bltin='; . , ? n istack @ ! + - choose nip'.split()

from sys import stdin

prog={}
wid={'main':0}
nums=[]

def makewid(w):
	if w not in wid:
		wid[w]=max(wid.values())+1
	return wid[w]

def makenum(w):
	if(w[0]=='_'): w=-int(w[1:])
	else: w=int(w)
	nums.append(w)
	return len(nums)-1

for x in stdin:
	w=x.split()
	if not w: continue
	df=[]
	for x in w[1:]:
		if x.startswith('#'):
			x=x[1:-1]+str(ord(x[-1]))
		flags=[]

		if '=' in x:
			flags.append('JZ')
			x=x.replace('=','',1)
		if len(x)>1 and '+' in x:
			flags.append('JP')
			x=x.replace('+','',1)
		if len(x)>1 and '-' in x:
			flags.append('JN')
			x=x.replace('-','',1)
		if len(x)>1 and '.' in x:
			flags.append('JMP')
			x=x.replace('.','',1)
		if "'" in x:
			flags.append('ZR')
			x=x[1:]
		if flags and flags!=['JMP']:
			f='|'.join(flags)
		else:
			if flags: f='JP|JN|JZ|JMP'
			else: f='JP|JN|JZ'

		if x in bltin:
			df.append("%s|CALL|BLTIN|%u"%(f,bltin.index(x)))
		elif x.isdigit() or (x.startswith('-') and x[1:].isdigit()):
			df.append("%s|CALL|NUM|%u"%(f,makenum(x)))
		else:
			df.append("%s|CALL|%u"%(f,makewid(x)))

	prog[w[0]]=df
	makewid(w[0])

print '#include <stdint.h>'
print '#include "prefix.h"'
for x,y in prog.items():
	print 'static uint16_t prog_%u[]={%s|JMP, JZ|JN|JP|BLTIN|0|JMP};'%(wid[x],', '.join(y))

print 'uint16_t *prog[]={%s};'%(','.join(['prog_%s'%x for x in range(0,len(wid))]))
print 'uint64_t prog_numbers[]={%s};'%(','.join([str(x) for x in nums]))

print 'char *prog_names[]={%s};'%(','.join(['"%s/%u"'%x for x in wid.items()]))

