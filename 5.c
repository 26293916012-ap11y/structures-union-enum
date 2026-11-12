#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define UPPER_BOUND 1000
#define EPSILON 1e-6

typedef struct {
    int RectID;
    float width;
    float height;
    float area;
    float peri;
} Rectangle;

void calculate_area(Rectangle *r) {
    r->area = r->width * r->height;
}

void calculate_perimeter(Rectangle *r) {
    r->peri = 2 * (r->width + r->height);
}

Rectangle* create_rectangle_array(int size) {
    Rectangle *rectangles = (Rectangle*) malloc(size * sizeof(Rectangle));
    return rectangles;
}

int check_input(Rectangle *r, int i) {
    if(r[i].RectID <= 0)
      return -1;
    if(r[i].width <= 0)
      return -1;
    if(r[i].height <= 0)
      return -1;
    return 0;
}

void add_rectangle(Rectangle *rectangles, int i, float width, float height) {
    rectangles[i].width = width;
    rectangles[i].height = height;
    calculate_area(&rectangles[i]);
}

void display_rectangles(Rectangle *rectangles, int size) {
    for (int i = 0; i < size; i++) {
        printf("Rectangle %d: Width = %.2f, Height = %.2f, Area = %.2f\n", 
               i, rectangles[i].width, rectangles[i].height, rectangles[i].area);
    }
}

int search_rectangle(Rectangle *r, int RectID, int size) {
    for (int i = 0; i < size; i++) {
        if(r[i].RectID == RectID)
          return i;
    }
    return -1;
}
  
int update_rectangle(Rectangle *r, int size, int RectID, float new_width, float new_height)
{
    int idx = search_rectangle( r, RectID, size);
    if (idx == -1)
        return -1;                          

    Rectangle temp;
    temp.RectID = RectID;
    temp.width  = new_width;
    temp.height = new_height;                 

    if (check_input(r, idx) == -1)
        return -1;                          

    r[idx] = temp;                      
    return 0;
}

int delete_students(Rectangle *r, int RectID, int size){
    int idx = search_rectangle( r, RectID, size);
    if (idx == -1)
        return -1;                      

    for (int i = idx; i < size - 1; i++)
        r[i] = r[i + 1];        

    (size)--;                         
    return 0;
}

void display_summary(Rectangle *r, int size){
    if (size == 0) {
        printf("No records.\n");
        return;
    }

    int best = 0, worst = 0;
    float total = 0;

    for (int i = 0; i < size; i++) {
        if (r[i].area > r[best].area) 
            best = i;
        if (r[i].area < r[worst].area)
            worst = i;
        total += r[i].area;
    }

    printf("Highest: %d %.2f\n", r[best].RectID, r[best].area);
    printf("Lowest : %d %.2f\n", r[worst].RectID, r[worst].area);
    printf("Total: %.2f\n", total);
    printf("Average: %.2f\n", total / size);
}

int check_square(Rectangle *r, int i){
    return fabs(r[i].height - r[i].width) <= EPSILON;
}

void display_sorted(Rectangle *r, int size){
Rectangle *copy = malloc(size * sizeof(Rectangle));

for(int i = 0; i < size; i++)
   copy[i] = r[i];

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size - 1; j++) {
            if (copy[j].area > copy[j+1].area) {
                Rectangle temp = copy[j];
                copy[j] = copy[j + 1];
                copy[j + 1] = temp;
            }
            else if (copy[j].area == copy[j+1].area && copy[j].RectID > copy[j+1].RectID){
                Rectangle temp = copy[j];
                copy[j] = copy[j + 1];
                copy[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < size; i++)
        printf("%d %s %d %.2f\n", copy[i].RectID, copy[i].width, copy[i].height, copy[i].area, copy[i].peri);
}

void free_rectangle_array(Rectangle *rectangles) {
    free(rectangles);
}

int main() {
    int size, ch, capacity;
    int id;
    float w, h;
    printf("Enter the number of rectangles.");
    scanf("%d", size);
    if(size < UPPER_BOUND)
      printf("OUT OF RANGE!");

    Rectangle *rectangles = create_rectangle_array(size);
    if (rectangles == NULL) {
        printf("Memory allocation failed.\n");
        return 0;
    }
 
    do {
        printf("\n1.Add  2.Display  3.Search  4.Update  5.Delete  6.Sorted by area\n");
        printf("7.Area summary  8.Square check  9.Filter by area  0.Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &ch) != 1)
            break;
 
        switch(ch){
            case 1:
                if (size == capacity) {
                    printf("Array is full.\n");
                    break;
                }
                printf("Enter rectangle ID: ");
                scanf("%d", &id);
                printf("Enter width: ");
                scanf("%f", &w);
                printf("Enter height: ");
                scanf("%f", &h);
                if (search_rectangle(rectangles, id, size) != -1) {
                    printf("Rectangle ID already exists.\n");
                }
                else {
                    rectangles[size].RectID = id;
                    add_rectangle(rectangles, size, w, h);
                    calculate_perimeter(&rectangles[size]);
                    if (check_input(rectangles, size) == -1) {
                        printf("Invalid input.\n");
                    }
                    else {
                        size++;
                        printf("Rectangle added.\n");
                    }
                }
                break;
            case 2:
                if (size == 0)
                    printf("No records.\n");
                else
                    display_rectangles(rectangles, size);
                break;
            case 3:
                printf("Enter rectangle ID: ");
                scanf("%d", &id);
                int idx = search_rectangle(rectangles, id, size);
                if (idx == -1)
                    printf("Rectangle ID not found.\n");
                else
                    printf("ID: %d, Width: %.2f, Height: %.2f, Area: %.2f, Perimeter: %.2f\n", rectangles[idx].RectID, rectangles[idx].width, rectangles[idx].height, rectangles[idx].area, rectangles[idx].peri);
                break;
            case 4:
                printf("Enter rectangle ID to update: ");
                scanf("%d", &id);
                printf("Enter new width: ");
                scanf("%f", &w);
                printf("Enter new height: ");
                scanf("%f", &h);
                if (w <= 0 || h <= 0) {
                    printf("Invalid input.\n");
                }
                else if (update_rectangle(rectangles, size, id, w, h) == -1) {
                    printf("Rectangle ID not found.\n");
                }
                else {
                    idx = search_rectangle(rectangles, id, size);
                    calculate_area(&rectangles[idx]);
                    calculate_perimeter(&rectangles[idx]);
                    printf("Rectangle updated.\n");
                }
                break;
            case 5:
                printf("Enter rectangle ID to delete: ");
                scanf("%d", &id);
                if (delete_students(rectangles, id, size) == -1) {
                    printf("Rectangle ID not found.\n");
                }
                else {
                    size--;
                    printf("Rectangle deleted.\n");
                }
                break;
            case 6:
                if (size == 0)
                    printf("No records.\n");
                else
                    display_sorted(rectangles, size);
                break;
            case 7:
                display_summary(rectangles, size);
                break;
            case 8:
                if (size == 0)
                    printf("No records.\n");
                for (int i = 0; i < size; i++) {
                    if (check_square(rectangles, i))
                        printf("Rectangle %d: Square\n", rectangles[i].RectID);
                    else
                        printf("Rectangle %d: Non-square\n", rectangles[i].RectID);
                }
                break;
            case 9:
                printf("Enter minimum area: ");
                float threshold;
                scanf("%f", &threshold);
                int shown = 0;
                for (int i = 0; i < size; i++) {
                    if (rectangles[i].area > threshold) {
                        printf("ID: %d, Width: %.2f, Height: %.2f, Area: %.2f\n", rectangles[i].RectID, rectangles[i].width, rectangles[i].height, rectangles[i].area);
                        shown++;
                    }
                }
                if (shown == 0)
                    printf("No rectangle has an area greater than %.2f.\n", threshold);
                break;
            case 0:
                break;
            default:
                printf("Invalid choice.\n");
        }
    } while (ch != 0);
 
    free_rectangle_array(rectangles);
    printf("Goodbye!\n");
    return 0;
}