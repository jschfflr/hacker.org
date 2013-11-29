content = []
with open('events.log') as f:
	content = f.readlines()

tmp = []
for line in content:
	row = line.strip().split('\t')
	if row[1] == 'stack':
		tmp.append( (row[0], row[2]) )
	elif row[1] == 'solved':
		with open(row[2] + '.stack', 'w') as f:
			for time,stack in tmp:
				f.write( stack + '\n')
			
		tmp = []