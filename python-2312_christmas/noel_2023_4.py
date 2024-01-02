#!/bin/python

##
 # @file  noel_2023_4.py
 # @brief Program to solve "Maman j’ai raté l’avion" channelge (christmas #4)
 #
 # @author Gwenael Saint-Genest <gwen@hooligan0.net>
 #
 # @license
 # This program is free software: you can redistribute it and/or modify it
 # under the terms of MIT license (see LICENSE.md)
 # This program is distributed WITHOUT ANY WARRANTY.
 #
from Game import Game

class Trap:
	def __init__(self, info):
		# Create vars
		self.x = 0
		self.y = 0
		self.size = 0
		self.code = ''
		# Extract trap description
		args = info.split(':')
		self.code = args[0]
		self.size = int(args[1])
		coords = args[2].split(';')
		self.x = int(coords[0])
		self.y = int(coords[1])

	def isTrapped(self, x, y):
		max_x = (self.x + self.size - 1)
		max_y = (self.y + self.size - 1)
		# Test if requested x is into x trap range
		valid_x = False
		if ((x >= self.x) and (x <= max_x)):
			valid_x = True
		# Test if requested y is into y trap range
		valid_y = False
		if ((y >= self.y) and (y <= max_y)):
			valid_y = True
		# If requested coordinate match X and Y
		if (valid_x and valid_y):
			return True
		return False

	def __str__(self):
		r = '{}'.format(self.code)
		r += ' size={}'.format(self.size)
		r += ' x={} y={}'.format(self.x, self.y)
		return r
	def __repr__(self):
		r = '{}'.format(self.code)
		r += ' size={}'.format(self.size)
		r += ' x={} y={}'.format(self.x, self.y)
		r += '\n'
		return r

##
 # @class Noel4
 # @brief Main class of the game
 #
class Noel4(Game):
	##
	 # @brief Default constructor (inherit from Game)
	 #
	def __init__(self):
		super().__init__('NOEL_2023_4')
		self.route = ""
		self.traps = []
		self.enemy_x = 0
		self.enemy_y = 0

	##
	 # @brief Get input data from server
	 #
	def prepare(self):
		# Get input data from server
		data = self.input()
		self.route = data['parcours']
		for t in data['pieges']:
			trap = Trap(t)
			self.traps.append(trap)

	##
	 # @brief Main function called to play the game
	 #
	def play(self):
		# Ask server for input data
		self.prepare()

		# Sort traps by their codes
		self.traps.sort(key=lambda x:x.code)
		print(self.traps)

		response = ''
		for mv in self.route:
			if (mv == 'H'):
				self.enemy_y += 1
			elif (mv == 'B'):
				self.enemy_y -= 1
			elif (mv == 'D'):
				self.enemy_x += 1
			print(' - {} : now at pos x={} y={}'.format(mv, self.enemy_x, self.enemy_y))
			for t in self.traps:
				if t.isTrapped(self.enemy_x, self.enemy_y):
					print(t)
					response += t.code

		print('Response : ' + response)
		# Send response to server
		game.output({'data': response})

game = Noel4()
game.play()
