content = []
with open('events.log') as f:
	content = f.readlines()

with open('samples.txt', 'w') as f:
  for line in content:
    row = line.strip().split('\t')
    if row[1] == 'samples':
      f.write( row[2] + '\t' + row[3] + '\n')