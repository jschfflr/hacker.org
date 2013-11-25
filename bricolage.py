import re

class Field:
  def __init__(self, width, height, board):
    self.width = width
    self.height = height
    self.board = board
    
  def get(self, x, y):
    return board[ x + y * width ]
    
  def __str__(self):
    string = ''
    for y in range(self.height - 1, 0 - 1, -1):
      for x in range(0, self.width):
        string += ' ' + self.board[x + y * self.width]
      
      string += '\n'
      
    return string
  
  @classmethod
  def load(cls, string):
    width = re.search('x=(\d+)', string).group(1)
    height = re.search('y=(\d+)', string).group(1)
    board = re.search('board=([^&]+)', string).group(1)
    
    return cls( int(width),int(height),board )

  
if __name__ == '__main__':
  field = Field.load('x=6&y=8&board=.bccb..bdbb...ddd...cbd.....c.....b.....b.......')
  print str(field)