#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>
/*
1.insert word
2.delete word
3.update word
4.insert line
5.delete line
*/
struct node
{
    char *s;
    int l;
    struct node *next;
};
struct undoRedo
{
    int op;
    struct node *data;
    struct undoRedo *next;
};
int lineNum = 0;
struct undoRedo *undo = NULL;
struct undoRedo *redo = NULL;
void createUndo(struct node *temp, int op)
{
    struct undoRedo *newnode = (struct undoRedo *)malloc(sizeof(struct undoRedo));
    newnode->op = op;
    newnode->data = (struct node *)malloc(sizeof(struct node));
    newnode->data->l = temp->l;
    newnode->data->s = (char *)malloc(strlen(temp->s) + 1);
    strcpy(newnode->data->s, temp->s);
    newnode->data->next = NULL;
    newnode->next = undo;
    undo = newnode;
}
void insert(struct node **head, char s[], int l, int pos, int up)
{
    struct node *temp = (*head);
    while (l - 1 && temp != NULL)
    {
        l--;
        temp = temp->next;
    }

    if (temp != NULL)
    {
        if (up != 0)
            createUndo(temp, 1);
        int len1 = strlen(temp->s);
        int len2 = strlen(s);

        if (len1 < pos)
        {
            printf("Data is inserted at the end of the line\n");
            pos = len1;
        }
        char *newStr = (char *)malloc(len1 + len2 + 3);
        strncpy(newStr, temp->s, pos);
        newStr[pos] = '\0';
        strcat(newStr, " ");
        strcat(newStr, s);
        strcat(newStr, " ");
        strcat(newStr, temp->s + pos);
        free(temp->s);
        temp->s = newStr;
    }
    else
    {
        printf("Line not found!!");
    }
}

int deleteFun(struct node **head, char s[], int l)
{
    int i;
    struct node *temp = (*head);
    while (l - 1 && temp != NULL)
    {
        temp = temp->next;
        l--;
    }
    if (temp != NULL)
    {
        int len1 = strlen(temp->s);
        int len = strlen(s);
        int j;
        int found = 0;
        for (i = 0; i <= len1 - len; i++)
        {
            found = 1;
            for (j = 0; j < len; j++)
            {
                if (temp->s[i + j] != s[j])
                {
                    found = 0;
                    break;
                }
            }
            if (found)
            {
                break;
            }
        }

        if (found)
        {
            createUndo(temp, 2);
            char *newstr = (char *)malloc(len1 - len + 1);
            int k = 0;

            for (j = 0; j < i; j++)
            {
                newstr[k++] = temp->s[j];
            }

            for (j = i + len; j < len1; j++)
            {
                newstr[k++] = temp->s[j];
            }

            newstr[k] = '\0';
            free(temp->s);
            temp->s = newstr;
            return i;
        }
    }
    return -1;
}

void display(struct node *head)
{
    struct node *temp = head;
    while (temp != NULL)
    {
        printf("%d %s", temp->l, temp->s);
        temp = temp->next;
    }
}

void deleteLine(struct node **head, int l, int flag)
{
    // clearRedo();
    if (l < 1)
    {
        printf("Please Enter valid line number!");
        return;
    }
    if (l == 1)
    {
        if (head != NULL)
        {
            lineNum--;
            if (flag == 0)
            {
                createUndo(*head, 5);
            }
            *head = (*head)->next;
            struct node *temp = (*head);
            while (temp)
            {
                temp->l -= 1;
                temp = temp->next;
            }
        }
        else
        {
            printf("/nNo line left!!\n");
        }
        return;
    }
    struct node *temp = (*head);
    struct node *prev = NULL, *save = NULL;
    // printf("%d ", l);
    while (l - 1 && temp != NULL)
    {
        prev = temp;
        temp = temp->next;
        l--;
    }
    // printf("%d ", l);
    if (temp != NULL)
    {
        lineNum--;
        if (prev != NULL)
        {
            if (flag == 0)
                createUndo(temp, 5);
            prev->next = temp->next;
            while (temp != NULL)
            {
                temp->l -= 1;
                temp = temp->next;
            }
            return;
        }
        else
        {
            if (flag == 0)
                createUndo(temp, 5);
            *head = (*head)->next;
            temp = *head;
            while (temp != NULL)
            {
                temp->l -= 1;
                temp = temp->next;
            }
            return;
        }

        if (temp->next != NULL)
        {
            save = temp;
            while (save != NULL)
            {
                save->l = save->l - 1;
                save = save->next;
            }
        }
        free(temp->s);
        free(temp);
    }
    else
    {
        printf("Please Enter valid line number!");
    }
}

void insertLine(struct node **head, char *s, int l, int flag)
{
    struct node *newNode = (struct node *)malloc(sizeof(struct node));
    struct node *temp = (*head);
    newNode->s = (char *)malloc(strlen(s) + 1);
    strcpy(newNode->s, s);
    newNode->next = NULL;
    lineNum++;
    if (l == 1)
    {
        newNode->l = 1;
        newNode->next = *head;
        *head = newNode;
        if (flag != 1)
            createUndo(*head, 4);
        temp = (*head)->next;
        while (temp != NULL)
        {
            temp->l++;
            temp = temp->next;
        }
        return;
    }

    struct node *prev = NULL;
    while (l - 1 && temp != NULL)
    {
        prev = temp;
        temp = temp->next;
        l--;
    }
    if (temp != NULL)
    {
        newNode->next = temp;
        newNode->l = temp->l;
        if (flag != 1)
            createUndo(newNode, 4);
        if (prev != NULL)
        {
            prev->next = newNode;
        }
        else
        {
            *head = newNode;
        }

        while (temp != NULL)
        {
            temp->l++;
            temp = temp->next;
        }
    }
    else
    {
        newNode->l = lineNum;
        if (flag != 1)
            createUndo(newNode, 4);

        if (prev != NULL)
        {
            // strcat(prev->s,"\n");
            prev->next = newNode;
            newNode->next = NULL;
        }
        else
        {
            *head = newNode;
            newNode->next = NULL;
        }
    }
}

void executeRedo(struct node **head)
{
    if (redo == NULL)
    {
        printf("No Redo left!!\n");
        return;
    }

    struct undoRedo *redoOp = redo;
    struct node *temp = *head;

    while (temp != NULL && temp->l != redoOp->data->l)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Line number not found for redo!\n");
        return;
    }

    switch (redoOp->op)
    {
        case 1:   
        case 2:   
        case 3:   
            strcpy(temp->s, redoOp->data->s);
            break;
        case 4:  
            insertLine(head, redoOp->data->s, redoOp->data->l, 1);
            break;
        case 5:   
            deleteLine(head, redoOp->data->l, 1);
            break;
        default:
            printf("Unknown redo operation!\n");
            return;
    }

    redo = redo->next;
    redoOp->next = undo;
    undo = redoOp;
}

void executeUndo(struct node **head)
{
    if (undo == NULL)
    {
        printf("No undo left!!\n");
        return;
    }

    struct undoRedo *undoOp = undo;
    struct node *temp = *head;

    while (temp != NULL && temp->l != undoOp->data->l)
    {
        temp = temp->next;
    }

    if (temp == NULL)
    {
        printf("Line number not found for undo!\n");
        return;
    }

    switch (undoOp->op)
    {
        case 1:   
        case 2:   
        case 3:  
            {
                struct undoRedo *redoOp = (struct undoRedo *)malloc(sizeof(struct undoRedo));
                redoOp->op = undoOp->op;
                redoOp->data = (struct node *)malloc(sizeof(struct node));
                redoOp->data->s = (char *)malloc(strlen(temp->s) + 1);
                strcpy(redoOp->data->s, temp->s);
                redoOp->data->l = temp->l;
                redoOp->next = redo;
                redo = redoOp;

                strcpy(temp->s, undoOp->data->s);
            }
            break;
        case 4:  
            deleteLine(head, undoOp->data->l, 1);
            break;
        case 5:  
            insertLine(head, undoOp->data->s, undoOp->data->l, 1);
            break;
        default:
            printf("Unknown undo operation!\n");
            return;
    }

    undo = undo->next;
    undoOp->next = redo;
    redo = undoOp;
}

void createNode(struct node **head, char s[])
{
    struct node *n = (struct node *)malloc(sizeof(struct node));
    n->s = (char *)malloc(strlen(s) + 1);
    strcpy(n->s, s);
    n->l = ++lineNum;
    n->next = NULL;
    if (*head == NULL)
    {
        *head = n;
        return;
    }
    else
    {
        struct node *temp = (*head);
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = n;
    }
}

int main(int argc, char *argv[])
{
    struct node *head = NULL;
    FILE *fp;
    char filepath[200];
    if (argc > 3)
    {
        printf("Error handling!");
        return 0;
    }
    else if (argc == 3)
    {
        snprintf(filepath, sizeof(filepath), "%s/%s", argv[2], argv[1]);

        fp = fopen(filepath, "r+");
        if (fp == NULL)
        {
            printf("no file exist!!\ncreating a new file...");
            fp = fopen(filepath, "w+");
            if (fp == NULL)
            {
                printf("Unable to create a new file!!");
                return 0;
            }
        }
    }
    else if (argc == 2)
    {
        fp = fopen(argv[1], "r+");
        if (fp == NULL)
        {
            printf("No file exist\nCreating a new file...");
            fp = fopen(argv[1], "w+");
            if (fp == NULL)
            {
                printf("Unable to create a new file!!");
                return 0;
            }
        }
    }
    else if (argc == 1)
    {
        fp = fopen("textFile.txt", "w+");
        if (fp == NULL)
        {
            printf("Unable to open a file!!");
            return 0;
        }
    }
    else
    {
        printf("Invalid input!!");
        return 0;
    }
    char s[200];
    while (fgets(s, sizeof(s), fp) != NULL)
    {
        createNode(&head, s);
    }
    int ch, flag = 1, line, pos;
    char str[100], str1[100], choice;
    struct undoRedo *dumy;
    while (flag)
    {
        system("cls");
        printf("\n1. Insert\n2. Delete\n3. Update\n4. Display\n5. Undo\n6. Redo\n7. Exit\nEnter your choice: ");
        scanf("%d", &ch);
        switch (ch)
        {
        case 1:
            printf("1. Insert New Line\n2. Insert in previous Data\nEnter your choice: ");
            scanf("%d", &choice);
            getchar();
            if (choice == 2)
            {
                printf("Enter data: ");
                scanf(" %[^\n]", str);
                str[strcspn(str, "\n")] = '\0';
                printf("Enter Line Number: ");
                scanf("%d", &line);
                printf("Enter Position: ");
                scanf("%d", &pos);
                insert(&head, str, line, pos, 1);
            }
            else if (choice == 1)
            {
                printf("Enter data: ");
                scanf(" %[^\n]", str);
                char newStr[strlen(str) + 3];
                strcpy(newStr, str);
                strcat(newStr, "\n");
                printf("Enter line number: ");
                scanf("%d", &line);
                insertLine(&head, newStr, line, 0);
            }
            else
            {
                printf("Wrong choice\n");
            }
            break;

        case 2:
            printf("1. Delete Line\n2. Delete in previous Data\nEnter your choice: ");
            scanf("%d", &choice);
            getchar();
            if (choice == 2)
            {
                printf("Enter data: ");
                scanf(" %[^\n]", str);
                str[strcspn(str, "\n")] = '\0';
                printf("Enter Line Number: ");
                scanf("%d", &line);
                deleteFun(&head, str, line);
            }
            else if (choice == 1)
            {
                printf("Enter Line Number: ");
                scanf("%d", &line);
                deleteLine(&head, line, 0);
            }
            else
            {
                printf("Wrong choice\n");
            }
            break;

        case 3:
            printf("Enter new data: ");
            getchar();
            scanf(" %[^\n]", str);
            str[strcspn(str, "\n")] = '\0';
            printf("Enter string which you want to replace: ");
            scanf(" %[^\n]", str1);
            str1[strcspn(str1, "\n")] = '\0';
            printf("Enter Line number: ");
            scanf("%d", &line);
            pos = deleteFun(&head, str1, line);
            if(pos!=-1)
            insert(&head, str, line, pos, 0);
            else
            printf("Not found!!");
            break;

        case 4:
            display(head);
            getch();
            break;

        case 5:
            executeUndo(&head);
            break;

        case 6:
            executeRedo(&head);
            break;

        case 7:
            flag = 0;
            break;
        case 8:
            dumy = redo;
            printf("\n\n");
            while (dumy)
            {
                printf("%d %s %d\n", dumy->data->l, dumy->data->s, dumy->op);
                dumy = dumy->next;
            }

            break;

        default:
            printf("Enter valid choice!\n");
            break;
        }
        if (ch != 4 && ch != 7)
        {
            display(head);
            getch();
        }
    }
    fclose(fp);
    if (argc == 3)
    {
        fp = fopen(filepath, "w+");
    }
    else if (argc == 2)
    {
        fp = fopen(argv[1], "w+");
    }
    else
    {
        fp = fopen("textFile.txt", "w+");
    }
    if (fp == NULL)
    {
        printf("Unable to open file in write mode!\n");
        return 1;
    }
    struct node *temp = head;
    while (temp != NULL)
    {
        fprintf(fp, "%s", temp->s);
        temp = temp->next;
    }
    fclose(fp);
    struct undoRedo *undoo = undo;
    while (undoo!=NULL)
    {
        free(undoo->data->s);
        free(undoo);
        undoo = undo->next;
    }
    struct undoRedo *redoo = redo;
    while (redoo!=NULL)
    {
        free(redoo->data->s);
        free(redoo);
        redoo = redo->next;
    }
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp->s);
        free(temp);
    }
    return 0;
}
