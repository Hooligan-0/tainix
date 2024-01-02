#!/bin/python

##
 # @file  noel_2023_3.py
 # @brief Program to solve "Die Hard" challenge (christmas 2023 #3)
 #
 # @author Gwenael Saint-Genest <gwen@hooligan0.net>
 #
 # @license
 # This program is free software: you can redistribute it and/or modify it
 # under the terms of MIT license (see LICENSE.md)
 # This program is distributed WITHOUT ANY WARRANTY.
 #
from Game import Game

class Enemy:
	def __init__(self, floor = 0):
		self.floor = floor;
	def __repr__(self):
		return str(self.floor)

##
 # @class Noel3
 # @brief Main class of the game
 #
class Noel3(Game):
	##
	 # @brief Default constructor (inherit from Game)
	 #
	def __init__(self):
		super().__init__('NOEL_2023_3')
		self.floor = 0
		self.enemies = []

	##
	 # @brief Get input data from server
	 #
	def prepare(self):
		# Get input data from server
		data = self.input()
		self.floor = int(data['john'])
		for e in data['ennemis']:
			new_guy = Enemy(e)
			self.enemies.append(new_guy)

	##
	 # @brief Main function called to play the game
	 #
	def play(self):
		# Ask server for input data
		self.prepare()

		print("== Start at floor {} -- {} ennemies".format(self.floor, len(self.enemies)))

		# Sort enemies by the floor where they are
		self.enemies.sort(key=lambda x:x.floor)

		# First, search enemies that are below John (and should be ignored)
		while len(self.enemies):
			# If this enemy is at john's floor (or upper), found first
			if self.enemies[0].floor >= self.floor:
				break
			print(' - Ignore enemy at floor {}'.format(str(self.enemies[0].floor)))
			self.enemies.pop(0)

		print(self.enemies)

		result = []
		# Insert floor where john starts
		result.append(self.floor)

		# Continue up to the last floor
		while self.floor < 100:
			if len(self.enemies) == 0:
				self.floor += 1
				continue
			target = self.enemies[0].floor
			if target > self.floor:
				self.floor += 1
				continue
			# Neutralize ennemy at current floor
			self.enemies.pop(0)
			result.append(target)
			print(' - Neutralize at floor {}'.format(target))
			# Enemies at 3 upper floor go down
			for i in range(3):
				if len(self.enemies) <= i:
					break
				efl = self.enemies[i].floor
				if (efl <= (self.floor + 3)):
					print('    - {} if now at {}'.format(str(efl), str(efl-1)))
					self.enemies[i].floor -= 1
		# If John has reach last floor without enemy
		if (result[-1] != 100):
			result.append(100)

		response = ''
		for e in result:
			if (response != ''):
				response += '-'
			response += str(e)
		
		# Send response to server
		print('Response ' + response)
		game.output({'data': response})

game = Noel3()
game.play()
