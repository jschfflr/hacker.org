content = []
with open('events.log') as f:
	content = f.readlines()

tmp = []
width = 0
height = 0
for line in content:
	row = line.strip().split('\t')
	if row[1] == 'state':
		width = int( row[2] )
		height = int( row[3] )
		tmp.append( (row[6] if len(row) == 7 else '', row[4] ) )
	elif row[1] == 'solved':
		with open(row[2] + '.level', 'w') as f:
			for path,state in tmp:
				board = ""
				board += path + '\n'
				board += row[4] + '\n'
				for y in range(height - 1, -1, -1):
					for x in range(0, width):
						board += state[x + y * width] + ' '
					
					board += '\n'
				
				board += '\n'
				f.write( board )
			
		tmp = []
		width = 0
		height = 0