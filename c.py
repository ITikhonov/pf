bltin='; . drop ? n istack'.split()

from sys import stdin

prog={}
wid={'main':0}
nums={0:0}

def makewid(w):
	if w not in wid:
		wid[w]=max(wid.values())+1
	return wid[w]

def makenum(w):
	if(w[0]=='_'): w=-int(w[1:])
	else: w=int(w)
	if w not in nums:
		nums[w]=max(nums.values())+1
	return nums[w]

for x in stdin:
	w=x.split()
	df=[]
	for x in w[1:]:
		flags=[]
		if '=' in x:
			flags.append('JZ')
			x=x.replace('=','')
		if '+' in x:
			flags.append('JP')
			x=x.replace('+','')
		if '-' in x:
			flags.append('JN')
			x=x.replace('-','')

		if flags:
			f='|'.join(flags)
		else:
			f='JP|JN|JZ'

		if x in bltin:
			df.append("%s|CALL|BLTIN|%u"%(f,bltin.index(x)))
		elif x.isdigit() or (x.startswith('_') and x[1:].isdigit()):
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
print 'uint32_t prog_numbers[]={%s};'%(','.join([str(x) for x in nums]))

