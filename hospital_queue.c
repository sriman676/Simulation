#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ==================== STRUCTURE DEFINITIONS ==================== */
typedef struct Patient
{
    int patientID;
    char name[50];
    char disease[50];
    int priority; // 1=High, 2=Medium, 3=Low
    char checkInTime[20];
} Patient;

typedef struct Node
{
    Patient data;
    struct Node *next;
} Node;

typedef struct Queue
{
    Node *front;
    Node *rear;
    int size;
} Queue;

/* ==================== GLOBAL VARIABLES ==================== */
Queue *mainQueue;
int patientCounter = 1000;
const char *PATIENT_FILE = "patients_log.txt";

/* ==================== FUNCTION DECLARATIONS ==================== */
Queue *createQueue();
void enqueue(Queue *q, Patient p);
Patient dequeue(Queue *q);
void displayQueue(Queue *q);
void savePatientToFile(Patient p);
void loadPatientsFromFile();
void displayMenu();
int isEmpty(Queue *q);
void addNewPatient(Queue *q);
void servePatient(Queue *q);
void searchPatient(Queue *q);
void displayStatistics();

/* ==================== QUEUE OPERATIONS ==================== */
Queue *createQueue()
{
    Queue *q = (Queue *)malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

int isEmpty(Queue *q)
{
    return q->size == 0;
}

void enqueue(Queue *q, Patient p)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->data = p;
    newNode->next = NULL;

    if (isEmpty(q))
    {
        q->front = newNode;
    }
    else
    {
        q->rear->next = newNode;
    }
    q->rear = newNode;
    q->size++;
    savePatientToFile(p);
    printf("\n✓ Patient %s added to queue (ID: %d)\n", p.name, p.patientID);
}

Patient dequeue(Queue *q)
{
    Patient empty = {0};
    if (isEmpty(q))
    {
        printf("\n✗ Queue is empty!\n");
        return empty;
    }

    Node *temp = q->front;
    Patient p = temp->data;
    q->front = q->front->next;
    q->size--;

    if (isEmpty(q))
    {
        q->rear = NULL;
    }

    free(temp);
    return p;
}

/* ==================== FILE OPERATIONS ==================== */
void savePatientToFile(Patient p)
{
    FILE *file = fopen(PATIENT_FILE, "a");
    if (file == NULL)
    {
        printf("Error: Cannot create log file!\n");
        return;
    }
    fprintf(file, "ID: %d | Name: %s | Disease: %s | Priority: %d | Time: %s\n",
            p.patientID, p.name, p.disease, p.priority, p.checkInTime);
    fclose(file);
}

void loadPatientsFromFile()
{
    FILE *file = fopen(PATIENT_FILE, "r");
    if (file == NULL)
    {
        printf("Log file not found. Starting fresh...\n");
        return;
    }

    char line[200];
    printf("\n--- Previous Session Log ---\n");
    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }
    fclose(file);
}

/* ==================== USER OPERATIONS ==================== */
void addNewPatient(Queue *q)
{
    Patient p;
    p.patientID = ++patientCounter;

    printf("\n--- Register New Patient ---\n");
    printf("Enter patient name: ");
    scanf(" %[^\n]", p.name);

    printf("Enter disease/complaint: ");
    scanf(" %[^\n]", p.disease);

    printf("Enter priority (1=High, 2=Medium, 3=Low): ");
    scanf("%d", &p.priority);

    if (p.priority < 1 || p.priority > 3)
    {
        p.priority = 3;
        printf("Invalid priority! Set to Low.\n");
    }

    strcpy(p.checkInTime, "10:00 AM"); // Default time
    enqueue(q, p);
}

void servePatient(Queue *q)
{
    if (isEmpty(q))
    {
        printf("\n✗ No patients in queue!\n");
        return;
    }

    Patient p = dequeue(q);
    printf("\n--- Serving Patient ---\n");
    printf("Patient ID: %d\n", p.patientID);
    printf("Name: %s\n", p.name);
    printf("Disease: %s\n", p.disease);
    printf("Priority Level: %d\n", p.priority);
    printf("Check-in Time: %s\n", p.checkInTime);
    printf("\n✓ Patient served successfully!\n");
}

void displayQueue(Queue *q)
{
    if (isEmpty(q))
    {
        printf("\n✗ Queue is empty!\n");
        return;
    }

    printf("\n--- Current Queue Status ---\n");
    printf("Total patients waiting: %d\n\n", q->size);
    printf("%-6s | %-20s | %-20s | %-10s\n", "ID", "Name", "Disease", "Priority");
    printf("--------|----------------------|----------------------|----------\n");

    Node *temp = q->front;
    int position = 1;
    while (temp != NULL)
    {
        printf("%-6d | %-20s | %-20s | %-10d\n",
               temp->data.patientID, temp->data.name, temp->data.disease, temp->data.priority);
        temp = temp->next;
        position++;
    }
}

void searchPatient(Queue *q)
{
    if (isEmpty(q))
    {
        printf("\n✗ Queue is empty!\n");
        return;
    }

    int searchID;
    printf("\nEnter Patient ID to search: ");
    scanf("%d", &searchID);

    Node *temp = q->front;
    int position = 1;
    while (temp != NULL)
    {
        if (temp->data.patientID == searchID)
        {
            printf("\n--- Patient Found ---\n");
            printf("Position in queue: %d\n", position);
            printf("Patient ID: %d\n", temp->data.patientID);
            printf("Name: %s\n", temp->data.name);
            printf("Disease: %s\n", temp->data.disease);
            printf("Priority: %d\n", temp->data.priority);
            return;
        }
        temp = temp->next;
        position++;
    }

    printf("\n✗ Patient with ID %d not found!\n", searchID);
}

void displayStatistics()
{
    printf("\n--- Queue Statistics ---\n");
    printf("Total patients in queue: %d\n", mainQueue->size);
    printf("Patients served so far: %d\n", patientCounter - 1000);
}

void displayMenu()
{
    printf("\n+======================================+\n");
    printf("|   HOSPITAL QUEUE MANAGEMENT SYSTEM   |\n");
    printf("+======================================+\n");
    printf("| 1. Register New Patient              |\n");
    printf("| 2. Serve Next Patient                |\n");
    printf("| 3. View Current Queue                |\n");
    printf("| 4. Search Patient                    |\n");
    printf("| 5. View Statistics                   |\n");
    printf("| 6. View Session Log                  |\n");
    printf("| 7. Exit                              |\n");
    printf("+======================================+\n");
    printf("Enter choice (1-7): ");
}

/* ==================== MAIN FUNCTION ==================== */
int main()
{
    mainQueue = createQueue();
    int choice;

    printf("\n+========================================+\n");
    printf("|  Welcome to Hospital Queue System     |\n");
    printf("+========================================+\n");

    loadPatientsFromFile();

    while (1)
    {
        displayMenu();
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addNewPatient(mainQueue);
            break;

        case 2:
            servePatient(mainQueue);
            break;

        case 3:
            displayQueue(mainQueue);
            break;

        case 4:
            searchPatient(mainQueue);
            break;

        case 5:
            displayStatistics();
            break;

        case 6:
            loadPatientsFromFile();
            break;

        case 7:
            printf("\n✓ Thank you for using Hospital Queue System!\n");
            printf("Exiting...\n");
            exit(0);

        default:
            printf("\n✗ Invalid choice! Please enter 1-7.\n");
        }
    }

    return 0;
}
