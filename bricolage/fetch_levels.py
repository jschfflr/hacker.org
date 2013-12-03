import urllib2, re

with open("levels.txt", "w") as f:
	level = 0;
	while(True):
		print 'Fetching level ' + str(level)
		r = urllib2.urlopen('http://hacker.org/brick/index.php?name=hakker1337&password=test1234&gotolevel=' + str(level))
		data = r.read();
		m = re.search('<param name="FlashVars" value="x=(\d+)&y=(\d+)&board=([^"]+)" />', data)
		if not m:
			break;
			
		width = m.group(1)
		height = m.group(2)
		board = m.group(3)
	
		f.write(width + "\t" + height + "\t" + board + "\n");
		level += 1
