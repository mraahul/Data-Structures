/*******************************************************************************************
	* File Name          : BInaryTree.c
	* Description        : Prog8130 - Assignment 2: Binary Tree
						   This program creats a binary tree to store the data and prints 
						   the tree in inorder traversal method without recursion.
	* Author:              Rahul Mishra
	* Date:                11th Feb 2019
	********************************************************************************************/

#include "pch.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structure defining a binary tree node.  Lower sorted values will go the the left, higher to the right.
struct nodeData {
	char name[30];				// character string of the data being stored in the node
	struct nodeData *left;		// pointer to the next node in the tree that is less than current node OR NULL if empty
	struct nodeData *right;		// pointer to the next node in the tree that is greater than current node OR NULL if empty
};

struct nodeData *head = NULL;	// head of the binary tree

// data to be stored in tree.  In this case there is just a character string but this could have contained more

const char *inputData[12] = {
	   { "hqebd"},
	   { "neyld"},
	   { "rvlhx"},
	   { "anmch"},
	   { "xcgqp"},
	   { "toxul"},
	   { "ubtou"},
	   { "ufibl"},
	   { "gspww"},		
	   { "lolch"},
	   { "rjqmv"},
	   { "smgnh"}
};

// The correct sorted order using a binary tree is:
//      anmch
//      gspww
//      hqebd
//      lolch
//      neyld
//      rjqmv
//      rvlhx
//      smgnh
//      toxul
//      ubtou
//      ufibl
//      xcgqp

// FUNCTION      : addToBinaryTree
// DESCRIPTION   : This function will store the data in newNode into a binary tree according to alphabetical order
// PARAMETERS    : newString - the string that is to be stored in the binary tree in alphabetica order
// RETURNS       : NONE
void addToBinaryTree(char *newString) {

	struct nodeData* newNode = (nodeData*)malloc(sizeof(nodeData));
	newNode->left = NULL;
	newNode->right = NULL;

	struct nodeData *currennodeData = NULL;  // a temporary pointer to move around the tree for comparing and referencing

	strcpy_s(newNode->name, 10, newString);

	if (head == NULL) {		//if the root is empty
		head = newNode;
	}
	else {
		currennodeData = head; //temp pointer ponts to current position
		while (1) {
			int ret = strcmp(newString, currennodeData->name);
			if (ret < 0)
			{
				if (currennodeData->left == NULL)
				{
					currennodeData->left = newNode; //if the left leaf is empty then fill that leaf with current data
					break;
				}	 
				else {
					currennodeData = currennodeData->left; //else temporary pointer points this leaf
				}
			}
			else {
				if (currennodeData->right == NULL)
				{
					currennodeData->right = newNode; //if the right leaf is empty then fill that leaf with current data
					break;
				}
				else {
					currennodeData = currennodeData->right; 
				}
			}// End of if-else
		}//End of while
	}
}

// FUNCTION      : printBinaryTree
// DESCRIPTION   : This function will print the entire binary tree out.  You can choose to print
//				   it using recursion but more marks will be awarded if you do it non recursively.
// PARAMETERS    : head - the root or parent node of the binary tree
// RETURNS       : NONE
void printBinaryTree(struct nodeData *head) {
	struct nodeData *current, *previous;
	
	if (head == NULL)
		return;

	current = head;
	while(current != NULL){

		if (current->left == NULL) {
			printf("%s\n", current->name);
			current = current->right;
		}
		else {
			previous = current->left;

			//find the inorder previous node of current; current is root of subtree and previous is the previous node
			while (previous->right != NULL && previous->right != current) {
				previous = previous->right;
			}

			if (previous->right == NULL) {
				previous->right = current;	//make current as right leaf of its inorder previous node
				current = current->left; 
			}
			else {
				previous->right = NULL;
				printf("%s\n",current->name);
				current = current->right;
			}
		}// End of if-else
	}//End of while
}



int main() {

	for (int i = 0; i < 12; i++)
		addToBinaryTree((char *)inputData[i]);

	printf("Printing data from Tree....\n\n");
	printBinaryTree(head);

	return 0;
}
