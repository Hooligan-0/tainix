#!/bin/python

##
 # @file  starter_5-obj.py
 # @brief Program to solve "Boucle foreach" challenge (starter_5)
 #
 # @author Gwenael Saint-Genest <gwen@hooligan0.net>
 #
 # @license
 # This program is free software: you can redistribute it and/or modify it
 # under the terms of MIT license (see LICENSE.md)
 # This program is distributed WITHOUT ANY WARRANTY.
##
from Game import Game

##
 # @class Starter5
 # @brief Main class of the game
 #
class Starter5(Game):
	##
	 # @brief Default constructor (inherit from Game)
	 #
	def __init__(self):
		super().__init__('STARTER_5')
		self.values = []

	##
	 # @brief Get input data from server
	 #
	def prepare(self):
		# Get input data from server
		data = self.input()
		# Do something with input data
		self.values = data['values'];

	##
	 # @brief Main function called to play the game
	 #
	def play(self):
		# Ask server for input data
		self.prepare()

		print("Input values ")
		print(self.values)

		result = 0
		for x in self.values:
			if x >= 5:
				result += x
		print("Response " + str(result))

		# Send response to server
		response = str(result)
		#game.output({'data': response})

game = Starter5()
game.play()
