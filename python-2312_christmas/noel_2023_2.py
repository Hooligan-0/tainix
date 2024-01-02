#!/bin/python

##
 # @file  noel_2023_2.py
 # @brief Program to solve "Le Grinch" challenge (christmas 2023 #2)
 #
 # @author Gwenael Saint-Genest <gwen@hooligan0.net>
 #
 # @license
 # This program is free software: you can redistribute it and/or modify it
 # under the terms of MIT license (see LICENSE.md)
 # This program is distributed WITHOUT ANY WARRANTY.
 #
from Game import Game

class Kid:
	def __init__(self, info):
		args = info.split('_')
		self.name = args[0]
		self.fear = int(args[1])

	def __str__(self):
		s = "{} ({})".format(self.name, str(self.fear))
		return s

##
 # @class Noel2
 # @brief Main class of the game
 #
class Noel2(Game):
	##
	 # @brief Default constructor (inherit from Game)
	 #
	def __init__(self):
		super().__init__('NOEL_2023_2')
		self.fear = 0
		self.time = 0
		self.kids = []

	##
	 # @brief Get input data from server
	 #
	def prepare(self):
		# Get input data from server
		data = self.input()
		self.fear = int(data['fear'])
		self.time = int(data['time'])
		for k in data['kids']:
			new_kid = Kid(k)
			self.kids.append(new_kid)

	##
	 # @brief Main function called to play the game
	 #
	def play(self):
		# Ask server for input data
		self.prepare()

		print("Start with {} factor, {} seconds".format(self.fear, self.time))

		remaining = self.time
		success = []
		for kid in self.kids:
			msg = ''
			if (self.fear > kid.fear):
				success.append(kid)
				remaining -= 3
				msg = 'Success {}'.format(kid)
			else:
				remaining -= 5
				msg = 'Failed {}'.format(kid)
			print(msg + " remaining "+str(remaining))
			if (remaining < 3):
				break

		response = ''
		if (len(success) > 0):
			for kid in success:
				initials = kid.name[0:1].upper()
				response += initials
		else:
			response = 'GRINCH'

		print("Response " + response)
		# Send response to server
		game.output({'data': response})

game = Noel2()
game.play()
