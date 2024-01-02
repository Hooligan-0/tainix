#!/bin/python

##
# @file  starter_1-proc.py
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

def main():
	game = Game('STARTER_1')
	data = game.input()

	# Extract input values
	side = int(data['side'])

	# Compute values
	perimeter = (side * 4)
	area = (side * side)

	response = perimeter + area

	print('Response : ' + str(response))

	# Send response to server
	game.output({'data': response})

main()
