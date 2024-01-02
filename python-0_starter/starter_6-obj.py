#!/bin/python

##
 # @file  starter_6-obj.py
 # @brief Program to solve "Fonction" challenge (starter_6)
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
 # @class Starter6
 # @brief Main class of the game
 #
class Starter6(Game):
	##
	 # @brief Default constructor (inherit from Game)
	 #
	def __init__(self):
		super().__init__('STARTER_6')
		self.values = []

	##
	 # @brief Get input data from server
	 #
	def prepare(self):
		# Get input data from server
		data = self.input()
		self.values = data['values']

	##
	 # @brief Main function called to play the game
	 #
	def play(self):
		# Ask server for input data
		self.prepare()

		print("Input values " + str(self.values))

		response = ''
		for x in self.values:
			if (x % 3):
				continue
			if (response != ''):
				response += '-'
			response += str(x)

		print("Response: " + response)

		# Send response to server
		game.output({'data': response})

game = Starter6()
game.play()
