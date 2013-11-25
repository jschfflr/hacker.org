import re

class Field:
  EMPTY = '.'
  
  def __init__(self, width, height, board):
    self.width = width
    self.height = height
    self.board = [ c for c in board ]
    
  def get(self, x, y):
    return self.board[ x + y * self.width ]
    
  def set(self, x, y, c):
    self.board[ x + y * self.width ] = c
    
  def __str__(self):
    string = ''
    for y in range(self.height - 1, 0 - 1, -1):
      for x in range(0, self.width):
        string += ' ' + self.board[x + y * self.width]
      
      string += '\n'
      
    return string
  
  #simple floodfill
  def remove(self, x, y, r = 0):
    c = self.get(x, y)
    if c == self.EMPTY:
      return x,y

    self.set(x, y, self.EMPTY)
    
    a1,b1 = a2,b2 = a3,b3 = a4,b4 = x,y
    
    if x > 0 and self.get(x - 1, y) == c:
      a1,b1 = self.remove(x - 1, y, r + 1)
    if x < self.width - 1 and self.get(x + 1, y) == c:
      a2,b2 = self.remove(x + 1, y, r + 1)
    if y > 0 and self.get(x, y - 1) == c:
      a3,b3 = self.remove(x, y - 1, r + 1)
    if y < self.height - 1 and self.get(x, y + 1) == c:
      a4,b4 = self.remove(x, y + 1, r + 1)
      
    return min(x, a1, a2, a3, a4),min(y, b1, b2, b3, b4)
  
  # update the field
  def update(self, left = 0, bottom = 1, right = False, top = False):
    changes = 0
    while True:
      for y in range( max( 1, bottom), min(top, self.height) if top  else self.height ):
        for x in range( max( 0, left), min(right, self.width) if right else self.width ):
          if self.get(x, y) != self.EMPTY and \
              self.get(x, y - 1) == self.EMPTY:
              
            self.set(x, y - 1, self.get(x, y))
            self.set(x, y, self.EMPTY)
            changes += 1
      
      if changes == 0:
        break
      
      changes = 0
  
  #check if empty
  def empty(self):
    for y in range(self.height - 1, 0 - 1, -1):
      for x in range(0, self.width):
        if self.get(x, y) != self.EMPTY:
          return False
     
    return True
  
  #perform a click
  def click(self, x, y):
    a,b = self.remove(x, y)
    self.update(a, b)
    if self.empty():
      print 'END'
  
  #load from string
  @classmethod
  def load(cls, string):
    width = re.search('x=(\d+)', string).group(1)
    height = re.search('y=(\d+)', string).group(1)
    board = re.search('board=([^&]+)', string).group(1)
    
    return cls( int(width),int(height),board )

  
if __name__ == '__main__':
  field = Field.load('x=8&y=5&board=.bbccbb..b.b.bb......c.......c..........')
  print str(field)
  field.click(6, 1)
  print str(field)
  field.click(4, 0)
  print str(field)
  field.click(1, 0)
  print str(field)
  