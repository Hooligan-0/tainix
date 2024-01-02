#!/bin/python

##
# @file  starter_2-obj.py
# @brief Program to solve "Variables et concatenation" challenge (starter_2)
#
# @author Gwenael Saint-Genest <gwen@hooligan0.net>
#
# @license
# This program is free software: you can redistribute it and/or modify it
# under the terms of MIT license (see LICENSE.md)
# This program is distributed WITHOUT ANY WARRANTY.
#
from Game import Game

##
# @class Starter2
# @brief Main class of the game
#
class Starter2(Game):
	##
	# @brief Default constructor (inherit from Game)
	#
	def __init__(self):
		super().__init__('STARTER_2')
		self.name = ""
		self.room = ""

	##
	# @brief Get input data from server
	#
	def prepare(self):
		# Get input data from server
		data = self.input()
		# Extract game inputs
		self.name = data['name']
		self.room = data['room']

	##
	# @brief Main function called to play the game
	#
	def play(self):
		# Ask server for input data
		self.prepare()

		response = self.name + " is in the " + self.room + "."
		print(response)
		# Send response to server
		game.output({'data': response})

game = Starter2()
game.play()
