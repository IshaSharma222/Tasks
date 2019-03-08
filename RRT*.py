#TASK 2 THE TURTLE AND THE HARE
#RRT-Star
import sys, math,random, pygame
from pygame import *     #import all attributes of pygame module  
from math import *       #import all attributes of math module

class Node(object) :
    def __init__(self, point, parent) :
	super(Node, self).__init__()    #initializes the parent class within the subclass
	self.point = point
	self.parent= parent
	self.dist=0    #initialize dist to zero
    def cost(self):
	cost=0
	p = self
	while p.parent!= None: #iterates over all the points to calculate the cost
	    cost+= p.dist
	    p= p.parent
	return cost

def dist(p1,p2):    #calculating distance between the two points
    return sqrt((p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1]))

cols=660          #gives the no. of rows of the window we are creating
rows= 660          #gives the no. of columns of the window we are creating
WinSize= [cols, rows]
pygame.init()     #initializes all imported pygame modules 
clock = pygame.time.Clock() #clock keeps track of the number of frames generated per second
screen= pygame.display.set_mode(WinSize)
delta = 15.0     #step value
rectscreen= screen.get_rect()   #returns a rectangle that will tell us the dimensions and locations of the screen
white= (255,255,255) # using colorName =(r,g,b) in pygame
black= (0,0,0)
red= (255,0,0)
green= (0,255,0)
blue= (0,0,255)
darkturquoise = (0,206,209)  
level1= [" tttttttttttttttttttt",        #list initialized to create a window in pygame of any design 
 	 " ....t.....t........t",
	 " t.....tttttt....tttt",
	 " t......tttttt...tttt",
	 " tt...t.............t",
	 " tt.......tt.........",
	 " ttttt......tt....ttt",
	 " t..........tt.t....t",
	 " tttttt........tttttt"
	 " t....t.........t.t.t"]

count= 0
obs= []
NODES= 6000
nbrhood= 20
rad= 10

def make_obstacles(list): #basically I am creating a screen with reasonable number of obstacles
    global obs
    obs= []
    Rows= len(list) 
    Cols= len(list[0])
    height= rectscreen.height/Rows
    length = rectscreen.width/Cols
    for a in range(Rows):
	for b in range(Cols):
	    if list[a][b] == 't' :
		obs.append(pygame.Rect((length*b, height*a),(length,height)))
    for rect in obs :
        pygame.draw.rect(screen,black, rect)

def collides(p):    #check if point collides with the obstacle
    for rect in obs:
    	p1=tuple(sum(x) for x in zip(p,(-5,0)))
    	p2=tuple(sum(x) for x in zip(p,(0,5)))
    	p3=tuple(sum(x) for x in zip(p,(5,0)))
    	if rect.collidepoint(p) == True or rect.collidepoint(p1) == True or rect.collidepoint(p2) == True or rect.collidepoint(p3) == True:
            return True
    return False

def get_random():   # generates a random point
    while True:
	p = random.random()*cols, random.random()*rows
	notCollision= collides(p)
	if notCollision== False:
	    return p

def step_function(p1,p2):   #returns a point at a step size away from the p1 in direction of p2
    if dist(p1,p2) < delta :
        return p2
    else :
	theta= atan2(p2[1]-p1[1], p2[0]-p1[0])
	return p1[0] + delta*cos(theta), p1[1] + delta*sin(theta)

def neighbourhood(p1,p2, rad):  #checks if the point p2 lies in the neighbourhood of p1
    distance= dist(p1,p2)
    if(distance<=rad):
	return True
    return False

def reset ():            #resets everything so that the user can again choose two points
    global count
    screen.fill(white)
    make_obstacles(level1)  # calls make_obstacles function to generate a window with obstacles as walls 
    count= 0

def main():
    global count
    initPointSet= False
    initialPoint= Node(None,None)
    goalPointSet= False
    goalPoint= Node(None, None)
    currentState = 'initial'      #initialized current state to initial
    sum=0
    nodes=[]
    reset()
    print "rectscreen height is = ", rectscreen.height
    print "rectscreen length is = ", rectscreen.width
    while True: 
	if currentState== 'initial':
	    pygame.display.set_caption('First select starting point and then Goal Point')
	    clock.tick(10) #means that for every second 10 frames should pass
	elif currentState == 'Goal':
	    currNode = goalNode
	    pygame.display.set_caption('Goal Found')
	    while currNode.parent !=None:
		pygame.draw.line(screen,red,currNode.point,currNode.parent.point,2)
		currNode= currNode.parent  #in this way the loop will iterate till it reaches the point with no parent i.e. the start point
		sum+=1
		if (sum%500)==0:    # using sum%500 I'll get a reasonable number of points on the final path and not too many points		
		   print(currNode.point)
	    optimizePhase= True
	elif currentState== 'optimize':
	    clock.tick(0.5)
	    pass
	elif currentState== 'Tree':
	    count+=1
	    pygame.display.set_caption('RRT_Star')
	    if count<NODES:      #iterates till the total no. of nodes is less than a specified value
	        foundNext= False
	        while foundNext== False:
		    rand = get_random()
		    parentNode= nodes[0]  
		    for p in nodes: 
		        if dist(p.point, rand) <= dist(parentNode.point, rand) :
			    newPoint = step_function(p.point, rand)
			    if collides(newPoint) == False:
			        parentNode = p
			        foundNext= True
	        newnode= step_function(parentNode.point, rand)
	        centerNode= parentNode
	                                 
	        parcost= parentNode.cost() # Finding the parent node
	        for p in nodes:
		    z= dist(p.point, newnode)
		    if (z<=nbrhood) and (z + p.cost()<= dist(parentNode.point, newnode)+ parcost):
		        parentNode= p
		        parcost= parentNode.cost()
		Newnode= Node(newnode,parentNode)
		Newnode.dist= dist(parentNode.point, newnode)
                	
	        nodes.append(Newnode)
		pygame.draw.line(screen,darkturquoise,parentNode.point,newnode)  #draws lines for the tree generated
	#//////////REWIRE//////////////////////
		for p in nodes:
		    d= dist(p.point, newnode)
		    e= Newnode.cost()
		    if( d<=nbrhood) and (d+e < p.cost()):
			pygame.draw.line(screen, white, p.parent.point, p.point) #the line is erased
			p.parent= Newnode
			p.dist=d
			pygame.draw.line(screen, darkturquoise,p.parent.point, p.point) #draw lines for new tree branches generated
	#//////////////////////////////////////  	
		if neighbourhood(newnode, goalPoint.point,rad):
		    currentState= 'Goal'
		    goalNode= nodes[len(nodes)-1]
		    print('goalFound', 'cost= ', goalNode.cost())
	    else:
	        print(" Sorry, path not found !")
	        return;


	for event in pygame.event.get():    #event is basically a mouse event in pygame
	    if event.type==QUIT or (event.type== KEYUP and event.key==K_ESCAPE):
	        sys.exit("Exiting")
	    if event.type== MOUSEBUTTONDOWN:
	        if currentState== 'initial':
		    if initPointSet == False:   #if initial point is not set
		        nodes =[]
		        if collides(event.pos) == False:   # i.e. no position has been clicked with the mouse  
			    print('initial point set: ' +str(event.pos))
			    initialPoint= Node(event.pos, None)
			    nodes.append(initialPoint)  #to be able to start in the centre
			    initPointSet= True
			    pygame.draw.circle(screen,red,initialPoint.point, rad)  #draws a circle of a given radius for initial point
		    elif goalPointSet == False:   #if goal point is not set
		        print('Goal point SET: '+str(event.pos))
		        if collides(event.pos)==False:  #i.e. no position has been clicked with the mouse
		            goalPoint= Node(event.pos, None)
			    goalPointSet = True #if goal point is set
			    pygame.draw.circle(screen, green,goalPoint.point, rad) #draws a circle of given radius for goal point
			    currentState= 'Tree'
	        else:
		    currentState= 'initial'
		    initPointSet= False
		    goalPointSet= False
		    reset()

        pygame.display.update()          #Update portions of the screen for software displays
        clock.tick(10000)

if __name__=='__main__':
    main()



