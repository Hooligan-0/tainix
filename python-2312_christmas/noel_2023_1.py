#!/bin/python

##
 # @file  noel_2023_1.py
 # @brief Program to solve "Le Pole Express" challenge (christmas 2023 #1)
 #
 # @author Gwenael Saint-Genest <gwen@hooligan0.net>
 #
 # @license
 # This program is free software: you can redistribute it and/or modify it
 # under the terms of MIT license (see LICENSE.md)
 # This program is distributed WITHOUT ANY WARRANTY.
 #
from Game import Game
from math import *

class Order:
	def __init__(self, info):
		args = info.split(',')
		# Extract each value from input string
		self.temp  = int(args[0])
		self.type  = args[1]
		self.spice = args[2]
		self.event = '';
		if (len(args) > 3):
			self.event = args[3]

	def __repr__(self):
		r = "Temp={}".format(self.temp)
		r += " {} ".format(self.type)
		r += " {} ".format(self.spice)
		if (self.event != ''):
			r += " {} ".format(self.event)
		return r

	def getTemp(self):
		temp = self.temp;
		temp += self.tempType()
		temp += self.tempSpice()
		# Update according to special events
		if (self.event == 'chocolat_brule'):
			temp -= 10
		elif (self.event == 'epice_surprise'):
			temp += 10
		elif (self.event == 'tasse_froide'):
			temp = (temp * 2)
		return temp

	def tempType(self):
		t = 0
		if (self.type == 'noir'):
			t = 5
		elif (self.type == 'au_lait'):
			t = 10
		elif (self.type == 'blanc'):
			t = 15
		elif (self.type == 'melange'):
			t = 12
		else:
			raise Exception("No type or unknown type")
		return t

	def tempSpice(self):
		t = 0
		if (self.spice == 'cannelle'):
			t = 4
		elif (self.spice == 'muscade'):
			t = 7
		elif (self.spice == 'piment'):
			t = 9
		elif (self.spice == 'vanille'):
			t = 1
		else:
			raise Exception("No Spice or unknown spice")
		return t

##
 # @class Noel1
 # @brief Main class of the game
 #
class Noel1(Game):
	##
	 # @brief Default constructor (inherit from Game)
	 #
	def __init__(self):
		super().__init__('NOEL_2023_1')
		self.orders = []

	##
	 # @brief Get input data from server
	 #
	def prepare(self):
		# Get input data from server
		data = self.input()
		for o in data['orders']:
			new_order = Order(o)
			self.orders.append(new_order)

	##
	 # @brief Main function called to play the game
	 #
	def play(self):
		# Ask server for input data
		self.prepare()

		print("Input " + str(self.orders))

		sum = 0;
		count = len(self.orders)
		for order in self.orders:
			sum += order.getTemp()

		avg = sum / count
		print("count="+str(count)+ " sum="+str(sum)+ " avg="+str(avg))

		response = str(ceil(avg))
		print("Response " + response)
		# Send response to server
		game.output({'data': response})

game = Noel1()
game.play()
