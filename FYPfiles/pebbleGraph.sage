class pebbleGraph(sage.graphs.digraph.DiGraph):
      '''
      An object that allows the pebble game algorithms to be run on a graph

      N (int) Number of nodes in the subject graph
      k (int) Number of pebbles on each node
      l (int) l+1 is the number of pebbles needed on two nodes in order to add an edge between them
      
      Creating an instance of this class will provide an empty N node graph with k pebbles on each node. 
	  The methods provided will allow a pebble game to be run. The user needs to specify the order in which the 
	  edges from the subject graph are added to this pebble graph.
      
	  Public Methods:
	  get_pebble_layout()
	  can_add_edge(i,j)
	  add_edge(i,j)
	  get_rejected_edges()
	  reject_edge(i,j)
	  find_pebble(i,j)
	  move_pebble(path)
	  attempt_add_edge(i,j)
	  output()
	  
      This class inherits from sage.graphs.digraph.DiGraph
	  This class is the base for componentGraph and colPebbleGraph classes
      '''
      def __init__(self,N,k,l):
      	  #k is the number of pebbles on each node
      	  self.k = k
	  #l+1 is the number of pebbles we need on two nodes in order to add an edge between them
	  self.l = l
	  self.N = N
	  #pebbleList keeps track of where the pebbles are in our graph
	  self.pebbleList = []
	  #Initializes the directed graph that the algorithm works on.
      	  sage.graphs.digraph.DiGraph.__init__(self, multiedges = True)
	  for i in range(N):
	      self.add_vertex()
	      #Put k pebbles on each node
	      self.pebbleList.append(self.k)
	  #We will need to keep track of any rejected edges so we can classify our graph when the algorithm has run its course
	  self.rej_edge_list = []

      #Checks the condition for adding an edge  
      def can_add_edge(self,i,j):
		  '''Returns True if an edge can be legally added between i and j, False otherwise'''
		  
      	  if (self.pebbleList[i] + self.pebbleList[j]) >= (self.l + 1):
	      return(True)
	  else:
	      return(False)

      #Adds an edge to directed graph and updates pebbleList
      def add_edge(self,i,j):
		  '''
		  Adds an edge from (int)i to (int)j and performs relevant pebble maintenance on the graph
		  
		  This method extends sage.graphs.digraph.DiGraph.add_edge(i,j)
		  '''
		  
      	  if self.pebbleList[i] == 0:
	     print("Cannot add an edge from", i, "to", j, "not enough pebbles on", i)
	  else:
	     sage.graphs.digraph.DiGraph.add_edge(self,i,j)
	     self.pebbleList[i] = self.pebbleList[i] - 1

      #Adds an edge to the rejected list
      def reject_edge(self,i,j):
	      '''Adds the edge from (int)i to (int)j into the list of rejected edges'''
		  
      	  self.rej_edge_list.append((i,j))

      #Tests the resuling pebble graph after the algorithm has run and reports to the user what the classification of their graph is
      def output(self):
		  '''
		  Provides a rigidity classification about the subject graph
		  
		  This method will only give useful output if the user has first attemted to add all edges from the subject graph into the pebble graph
		  '''
		  
	  print("Caution: This function will not give a correct output unless all edges from the original graph have been considered for addition to the pebble graph. If you are running the runPebbleGame() function, disregard this warning.")
	  print("Your graph can be classified as: ")
	  if sum(self.pebbleList) == self.l:
	     if self.rej_edge_list == []:
	     	print("Well constrained")
	     else:
	        print("Over constrained")
	  else:
	     if self.rej_edge_list == []:
	     	print("Under constrained")
	     else:
		print("Other. This means that, overall, your graph is under contrained. However it contains a component that is over constrained")

      #Moves a pebble along a path, reversing edges and updating the pebbleList as it goes
      def move_pebble(self,path):
		  '''
		  Moves a pebble along a (list<int>) path
		  
		  The path leads from where a pebble was found to where it is needed.  This method takes a pebble along that path and 
		  updates pebble positions along the way.
		  '''
      	  for i in range(len(path)-1):
	     j = i + 1
	     self.pebbleList[path[j]] = self.pebbleList[path[j]] + 1
	     self.delete_edge(path[j],path[i])
	     self.add_edge(path[i],path[j])

      #If trying to add an egde but not enough pebbles are present, this method	will find the path to a pebble using a depth first search or will return that no pebble could be found
      def find_pebble(self,i,j):
		'''Finds a path to a pebble to add to either node (int)i or node (int)j in order to make up l pebbles between i and j'''
      	  visited = [i,j]
	  #We can't add a pebble if a node(i) already has k pebbles present, so	we try to find a pebble for node j 
	  if self.pebbleList[i] == self.k:
	     path_to_pebble = self._DFS(j,visited)
	  #Similar to previous comment
	  elif self.pebbleList[j] == self.k:
	     path_to_pebble = self._DFS(i,visited)
	  else:
	     #This is where neither node has k pebbles present so we search both nodes in turn
	     path_to_pebble = self._DFS(i,visited)
	     if path_to_pebble == []:
	     	path_to_pebble = self._DFS(j,visited)
	     else:
		pass
	  return(path_to_pebble)

      #This is the method that does the actual searching for pebbles, i is the node that needs a pebble, visited will be a list with i and j in it where i and j are the nodes we are trying to add an edge between 
      def _DFS(self,i,visited):
      	  #pebble_at_vert will tell us where we have found a pebble
	  pebble_at_vert = "none"
	  #reconstruct will hold the information that allows us to navigate back to node i when we find a pebble
	  reconstruct = []
	  #Initially populate reconstruct with "none" strings
	  for d in range(len(self.vertices())):
	      reconstruct.append("none")
	  #The stack will tell us where we need to visit next
	  stack = []
	  #Initial population of stack with out neighbours of i
	  for vert in self.neighbors_out(i):
	      #If vert is alredy in stack then we have a multi edge and we don't need another copy of vert in the stack
	      if vert not in stack and vert not in visited:
	      	 stack.append(vert)
		 #Recoding that we came to vert from i so thats where we return	to when retracing our path
	      	 reconstruct[vert] = i
	      else:
		 pass
	  #When the stack is empty this means we have visited all nodes	discoverable from i and we can stop the search
	  while len(stack) > 0:
	      v = stack.pop()
	      #check that we have not visited v already
	      visited.append(v)
	      #If the below condition is met then we have found a pebble
	      if self.pebbleList[v] > 0:
		 pebble_at_vert = v
		 #No need to keep searching so break the while loop
		 break
	      #If we don't find a pebble at v then we search all of v's out neighbours
	      else:
		 for w in self.neighbors_out(v):
		     if w not in stack and w not in visited:
		    	stack.append(w)
			#We got to w from v so thats the way we need to go if we wanted to find our way back to i
			reconstruct[w] = v
		     else:
			pass 
 	  #If we have performed our search and pebble_at vert doesn't have a value then there is no pebble availible and we return an empty path
	  if pebble_at_vert == "none":
	      return([])
	  #Otherwise we have found a pebble and we can reconstruct our path from pebble_at_vert to i using our reconstruct list
	  else:
	      path = []
	      next_node = pebble_at_vert
	      while next_node != "none":
	      	 path.append(next_node)
		 next_node = reconstruct[next_node]
	      return(path)

      #This method takes two vertices and tries to add an edge between them by finding pebbles etc... It will reject the edge if too few pebbles are found
      def attempt_add_edge(self,i,j):
		'''
		Performs an edge addition without the user having to run the pebble movement/finding methods.
		
		If edge ij can be added to the pebble graph, this method performs all necessary pebble movements 
		in order to add the edge then adds the edge, otherwise rejects the edge
		'''
		
      	  layout = self.get_pebble_layout()
	  #Sometimes all L+1 pebbles are on node j. In this case the following clause makes sure that we never add an edge ij where there are no pebbles on i
	  if (self.can_add_edge(i,j)) and layout[i] == 0:
	     temp = i
	     i = j
	     j = temp
	  else:
	     pass
	  #While we cannot add an edge, keep finding and moving pebbles
	  while not (self.can_add_edge(i,j)):
	     pathToPebb = self.find_pebble(i,j)
	     #If, at any stage, no pebble is found edge ij needs to be rejected
	     if pathToPebb == []:
		break
	     else:
		self.move_pebble(pathToPebb)
	  if (self.can_add_edge(i,j)):
	     self.add_edge(i,j)
	  else:
	     self.reject_edge(i,j)

      #Accessor methods

      def get_pebble_layout(self):
		  '''
		  Returns the number of pebbles on each node in list form

		  For example: [1,2,3] there is 1 pebble on node 0, 2 pebbles on node 1 and 3 pebbles on node 2.
		  '''
	  
      	  return(self.pebbleList)

      def get_rejected_edges(self):
		  '''Returns a list of rejected edges'''
	  
      	  return(self.rej_edge_list)
