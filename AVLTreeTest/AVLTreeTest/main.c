#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct AVLNode 
{
	int val;
	int height;
	struct AVLNode* left;
	struct AVLNode* right;
};

int returnHeight(struct AVLNode* head)
{
	return 1 + max(height(head->left), height(head->right));
}

void updateHeight(struct AVLNode* head)
{
	head->height = returnHeight(head);
}

void rotateLeft(struct AVLNode** head)
{
	if ((*head)->right != NULL)
	{
		struct AVLNode* temp = (*head)->right;
		struct AVLNode* tempLeft = (*head);
		struct AVLNode* tempLeftRight = (*head)->right->left;

		temp->left = tempLeft;
		temp->left->right = tempLeftRight;

		updateHeight(tempLeft);
		updateHeight(temp);

		*head = temp;
	}
}

void rotateRight(struct AVLNode** head)
{
	if ((*head)->left != NULL)
	{
		struct AVLNode* temp = (*head)->left;
		struct AVLNode* tempRight = (*head);
		struct AVLNode* tempRightLeft = (*head)->left->right;

		temp->right = tempRight;
		temp->right->left = tempRightLeft;

		updateHeight(tempRight);
		updateHeight(temp);

		*head = temp;
	}
}

int height(struct AVLNode* head)
{
	if (head == NULL)
	{
		return 0;
	}
	else
	{
		return head->height;
	}
}

int getBalance(struct AVLNode* head)
{
	if (head == NULL)
	{
		return 0;
	}
	return height(head->left) - height(head->right);
}

bool isUnbalanced(struct AVLNode* head)
{
	if (getBalance(head) < -1 || getBalance(head) > 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void balanceInsert(struct AVLNode** head, int val)
{
	if (isUnbalanced(*head))
	{
		int balance = getBalance(*head);

		if (balance > 1 && val < (*head)->left->val)
		{
			rotateRight(head);
		}
		else if (balance > 1 && val > (*head)->left->val)
		{
			rotateLeft(&(*head)->left);
			rotateRight(head);
		}
		else if (balance < -1 && val < (*head)->right->val)
		{
			rotateRight(&(*head)->right);
			rotateLeft(head);
		}
		else if (balance < -1 && val > (*head)->right->val)
		{
			rotateLeft(head);
		}
	}
}

void balanceDelete(struct AVLNode** head)
{
	if (isUnbalanced(*head))
	{
		int balance = getBalance(*head);

		if (balance > 1 && getBalance((*head)->left) >= 0)
		{
			rotateRight(head);
		}
		else if (balance > 1)
		{
			rotateLeft(&(*head)->left);
			rotateRight(head);
		}
		else if (balance < -1 && getBalance((*head)->right) > 0)
		{
			rotateRight(&(*head)->right);
			rotateLeft(head);
		}
		else if (balance < -1)
		{
			rotateLeft(head);
		}
	}
}

int inOrderSuccessor(struct AVLNode** head)
{
	struct AVLNode** temp = &(*head)->right;
	int val = (*temp)->val;
	while ((*temp)->left != NULL)
	{
		temp = &(*temp)->left;
		val = (*temp)->val;
	}
	free(*temp);
	*temp = NULL;
	return val;
}

struct AVLNode* createNode(int val)
{
	struct AVLNode* vertex = (struct AVLNode*)malloc(sizeof(struct AVLNode));
	if (vertex)
	{
		vertex->val = val;
		vertex->height = 1;
		vertex->left = NULL;
		vertex->right = NULL;
	}
	return vertex;
}

void insertNode(struct AVLNode** head, int val)
{
	if (*head == NULL)
	{
		*head = createNode(val);
	}
	else
	{
		if (val <= (*head)->val)
		{
			insertNode(&(*head)->left, val);
			updateHeight(*head);

			balanceInsert(head, val);
		}
		else
		{
			insertNode(&(*head)->right, val);
			updateHeight(*head);

			balanceInsert(head, val);
		}
	}
}

void deleteNode(struct AVLNode** head, int val)
{
	if (*head == NULL)
	{
		return;
	}
	else if ((*head)->val == val)
	{
		if ((*head)->left == NULL && (*head)->right == NULL)
		{
			free(*head);
			*head = NULL;
		}
		else if ((*head)->left != NULL && (*head)->right == NULL)
		{
			struct AVLNode* temp = *head;
			*head = (*head)->left;
			free(temp);
		}
		else if ((*head)->left == NULL && (*head)->right != NULL)
		{
			struct AVLNode* temp = *head;
			*head = (*head)->right;
			free(temp);
		}
		else
		{
			(*head)->val = inOrderSuccessor(head);
			updateHeight(*head);

			balanceDelete(head);
		}
	}
	else
	{
		if (val <= (*head)->val)
		{
			deleteNode(&(*head)->left, val);
			updateHeight(*head);

			balanceDelete(head);
		}
		else
		{
			deleteNode(&(*head)->right, val);
			updateHeight(*head);

			balanceDelete(head);
		}
	}
}

int main(void)
{
	struct AVLNode* head = NULL;

	int h = height(head);

	insertNode(&head, 10);
	insertNode(&head, 20);
	insertNode(&head, 30);
	insertNode(&head, 5);
	insertNode(&head, 8);
	insertNode(&head, 3);
	insertNode(&head, 2);
	insertNode(&head, 9);

	deleteNode(&head, 20);

	return 0;
}