# Instant-Runoff-Voting-C
# Instant Runoff Voting (IRV) System – C Implementation

## Overview

This project implements the Australian House of Representatives
preferential voting system using the Instant Runoff Voting (IRV) algorithm.

The program reads candidate names and voter preference rankings,
then simulates multi-round elimination until a candidate obtains
more than 50% of valid votes.

## Features

- Multi-round vote counting
- Automatic elimination of lowest-ranked candidate
- Vote redistribution based on next preferences
- Percentage calculation per round
- Alphabetical tie-breaking (Stage 3 enhancement)
- Structured modular design

## Technical Highlights

- 2D array manipulation
- Dynamic vote redistribution logic
- Custom insertion sort implementation
- Pointer-based parameter passing
- Structured modular programming in C

## Algorithm Complexity

Each round:
- Vote counting: O(n × m)
- Redistribution: O(n × m)
- Sorting (Stage 3): O(m²)

Where:
- n = number of voters
- m = number of candidates

## Project Structure
