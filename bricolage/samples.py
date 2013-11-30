content = []
with open('events.log') as f:
	content = f.readlines()

samples = []
possibilities = []

with open('samples.txt', 'w') as f:
  for line in content:
    row = line.strip().split('\t')
    if row[1] == 'samples':
      samples = row
    elif row[1] == 'possibilities':
      possibilities = row
    elif row[1] == 'solved':
      f.write( samples[2] + '\t' + samples[3] + '\t' + possibilities[2] + '\t' + possibilities[3] + '\n')