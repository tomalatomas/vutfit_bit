//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     Red-Black Tree - public interface tests
//
// $NoKeywords: $ivs_project_1 $black_box_tests.cpp
// $Author:     Tomas Tomala <xtomal02@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file black_box_tests.cpp
 * @author Tomas Tomala
 * 
 * @brief Implementace testu binarniho stromu.
 */

#include <vector>

#include "gtest/gtest.h"

#include "red_black_tree.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy Red-Black Tree, testujte nasledujici:
// 1. Verejne rozhrani stromu
//    - InsertNode/DeleteNode a FindNode
//    - Chovani techto metod testuje pro prazdny i neprazdny strom.
// 2. Axiomy (tedy vzdy platne vlastnosti) Red-Black Tree:
//    - Vsechny listove uzly stromu jsou *VZDY* cerne.
//    - Kazdy cerveny uzel muze mit *POUZE* cerne potomky.
//    - Vsechny cesty od kazdeho listoveho uzlu ke koreni stromu obsahuji
//      *STEJNY* pocet cernych uzlu.
//============================================================================//
class EmptyTree : public ::testing::Test{
protected:
	BinaryTree tree;
	
};

class NonEmptyTree : public ::testing::Test{
protected:
	virtual void SetUp(){
		int values[] = {3,5,2,6,1,9,7,4,8};

		for(int i=0; i<9; ++i){
			tree.InsertNode(values[i]);
		}
	}
	BinaryTree tree;
};

class TreeAxioms : public ::testing::Test{
protected:
	virtual void SetUp(){
		int values[] = {3,5,2,6,1,9,7,4,8};

		for(int i=0; i<9; ++i){
			tree.InsertNode(values[i]);
		}
	}
	BinaryTree tree;
};

TEST_F(EmptyTree, Root){
	EXPECT_TRUE(tree.GetRoot()==NULL);
}

TEST_F(EmptyTree, FindNode){
	EXPECT_TRUE(tree.FindNode(-10)==NULL);
	EXPECT_TRUE(tree.FindNode(0)==NULL);
	EXPECT_TRUE(tree.FindNode(10)==NULL);
}

TEST_F(EmptyTree, InsertNode){
	EXPECT_TRUE(tree.GetRoot()==NULL);

	std::pair<bool, BinaryTree::Node_t*> insertedFirst=tree.InsertNode(10);
	EXPECT_FALSE(tree.GetRoot()==NULL);
	EXPECT_TRUE(tree.GetRoot()==insertedFirst.second);
	EXPECT_TRUE(insertedFirst.second->pParent==NULL);
	EXPECT_TRUE(insertedFirst.second->color==BLACK);

	std::pair<bool, BinaryTree::Node_t*> insertedSecond=tree.InsertNode(20);
	EXPECT_TRUE(tree.GetRoot()==insertedFirst.second);
	EXPECT_TRUE(insertedSecond.second->pParent==insertedFirst.second);
	EXPECT_TRUE(insertedSecond.second->color==RED);
}
TEST_F(EmptyTree, DeleteNode){
	EXPECT_FALSE(tree.DeleteNode(0));
	std::pair<bool, BinaryTree::Node_t*> toDelete=tree.InsertNode(10);
	EXPECT_TRUE(tree.DeleteNode(10));
	EXPECT_FALSE(tree.DeleteNode(10));
}

TEST_F(NonEmptyTree, Root){
	Node_t *root =tree.GetRoot();
	ASSERT_TRUE(root!=NULL);
	ASSERT_TRUE(root->pParent==NULL);
	EXPECT_TRUE(root->key==3);
}

TEST_F(NonEmptyTree, FindNode){
	EXPECT_TRUE(tree.FindNode(5)!=NULL);
	EXPECT_TRUE(tree.FindNode(8)!=NULL);
	EXPECT_TRUE(tree.FindNode(1)!=NULL);
	EXPECT_TRUE(tree.FindNode(0)==NULL);
	EXPECT_TRUE(tree.FindNode(-5)==NULL);
	EXPECT_TRUE(tree.FindNode(-8)==NULL);
	EXPECT_TRUE(tree.FindNode(-1)==NULL);
	EXPECT_TRUE(tree.FindNode(10)==NULL);
}

TEST_F(NonEmptyTree, DeleteNode_Root){
	ASSERT_FALSE(tree.GetRoot()==NULL);
	EXPECT_TRUE(tree.GetRoot()->key==3);
	EXPECT_TRUE(tree.DeleteNode(3));
	EXPECT_TRUE(tree.GetRoot()->key!=3);
}

TEST_F(NonEmptyTree, DeleteNode_Forward){
	int values[] = {3,5,2,6,1,9,7,4,8};

	for(int i=0; i<9; ++i){
		EXPECT_TRUE(tree.DeleteNode(values[i]));
		EXPECT_FALSE(tree.DeleteNode(values[i]));
	}
	EXPECT_TRUE(tree.GetRoot()==NULL);
}

TEST_F(NonEmptyTree, InsertNode){
	ASSERT_TRUE(tree.GetRoot()!=NULL);

	//Adding non-existing node
	Node_t *nonExisting =tree.FindNode(6);
	std::pair<bool, BinaryTree::Node_t*> nonExistingAdded=tree.InsertNode(10);
	EXPECT_TRUE(nonExistingAdded.first==true);
	EXPECT_FALSE(nonExistingAdded.second==nonExisting);

	//Adding existing node
	Node_t *existing =tree.FindNode(6);
	std::pair<bool, BinaryTree::Node_t*> existingAdded=tree.InsertNode(6);
	EXPECT_TRUE(existingAdded.first==false);
	EXPECT_TRUE(existingAdded.second==existing);
}

TEST_F(TreeAxioms, FirstAxiom){
	std::vector<BinaryTree::Node_t *> leafs;
	tree.GetLeafNodes(leafs);
	for(Node_t *leaf: leafs){
		EXPECT_TRUE(leaf->color==BLACK);
	}

}

TEST_F(TreeAxioms, SecondAxiom){
	std::vector<BinaryTree::Node_t *> nodes;
	tree.GetAllNodes(nodes);
	for(Node_t *node: nodes){
		if(node->color==RED){
			EXPECT_TRUE(node->pLeft->color==BLACK);
			EXPECT_TRUE(node->pLeft->color==BLACK);

		}
	}
}

TEST_F(TreeAxioms, ThirdAxiom){
	std::vector<BinaryTree::Node_t *> leafs;
	tree.GetLeafNodes(leafs);
	int paths[leafs.size()];
	int current=0;
	for(Node_t *leaf: leafs){
		//printf("-----Uzel číslo:%d-----\n",current);
		int currentsPath=0;
		while(leaf->pParent!=NULL){
			if(leaf->pParent->color==BLACK)currentsPath++;
			leaf=leaf->pParent;
			//printf("%d\n",currentsPath);
		}
		paths[current]=currentsPath;
		current++;
	}
	current=0;
	for(int i=0;i<leafs.size()-1;i++){
		//printf("%d==%d\n",paths[i],paths[i+1]);
		EXPECT_TRUE(paths[i]==paths[i+1]);
	}	

}



/*** Konec souboru black_box_tests.cpp ***/
