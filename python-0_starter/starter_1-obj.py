#!/bin/python

##
# @file  starter_1-obj.py
# @brief Program to solve "Variables et calcul" challenge (starter_1)
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
# @class Starter1
# @brief Main class of the game
#
class Starter1(Game):
	##
	# @brief Default constructor (inherit from Game)
	#
	def __init__(self):
		super().__init__('STARTER_1')
		self.side = 0

	##
	# @brief Get input data from server
	#
	def prepare(self):
		# Get input data from server
		data = self.input()
		self.side = int(data['side'])

	#
	# @brief Main function called to play the game
	#
	def play(self):
		# Ask server for input data
		self.prepare()

		# Compute values
		perimeter = (self.side * 4)
		area = (self.side * self.side)

		response = perimeter + area

		print('Response : ' + str(response))
		# Send response to server
		game.output({'data': response})

game = Starter1()
game.play()
