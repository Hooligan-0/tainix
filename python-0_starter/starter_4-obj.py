#!/bin/python

##
# @file  starter_4-obj.py
# @brief Program to solve "Boucle for" challenge (starter_4)
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
 # @class Starter4
 # @brief Main class of the game
 #
class Starter4(Game):
	##
	 # @brief Default constructor (inherit from Game)
	 #
	def __init__(self):
		super().__init__('STARTER_4')
		self.stop = 0

	##
	 # @brief Get input data from server
	 #
	def prepare(self):
		# Get input data from server
		data = self.input()
		# Do something with input data
		self.stop = int(data['stop'])

	##
	 # @brief Main function called to play the game
	 #
	def play(self):
		# Ask server for input data
		self.prepare()
		
		print("Input "+ str(self.stop))
		result = 0
		for i in range(self.stop):
			result += i

		print("Result "+ str(result))

		# Send response to server
		response = str(result)
		game.output({'data': response})

game = Starter4()
game.play()
