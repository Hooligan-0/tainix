#!/bin/python

##
# @file  starter_3-obj.py
# @brief Program to solve "Condition" challenge (starter_3)
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
 # @class Starter3
 # @brief Main class of the game
 #
class Starter3(Game):
	##
	 # @brief Default constructor (inherit from Game)
	 #
	def __init__(self):
		super().__init__('STARTER_3')
		self.number = 0

	##
	 # @brief Get input data from server
	 #
	def prepare(self):
		# Get input data from server
		data = self.input()
		# Extract input data
		self.number = data['number']

	##
	 # @brief Main function called to play the game
	 #
	def play(self):
		# Ask server for input data
		self.prepare()

		response = ''
		if (self.number % 3):
			response = 'NO'
		else:
			response = 'YES'

		print("Input value "+ str(self.number) + " Result "+ response)

		# Send response to server
		game.output({'data': response})

game = Starter3()
game.play()
