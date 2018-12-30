#include<cstdio>
#include<vector> 
#include<time.h> 

using namespace std; 

struct Node {
    int key; 
    int value;
    int level; 
    struct Node **next; 
}; 

class SkipList {
    private:

        int max_level;
        struct Node* head; 
        struct Node* tail; 

    public: 
        
        // 构造函数
        SkipList(int l) {
            max_level = l; 
            head = new struct Node; 
            tail = new struct Node; 

            head->next = new struct Node *[max_level]; 
            for ( int i = 0 ; i < max_level ; i++ ) {
                head->next[i] = tail; 
            }
        } 

        // 析构函数  
        ~SkipList(){
            struct Node *curr = head->next[0]; 
            while ( curr != tail ) {
                struct Node *tmp = curr->next[0];  
                delete curr->next;
                delete curr;
                curr = tmp; 
            } 
            delete head->next; 
            delete head; 
            delete tail; 
        }
         
        // 插入节点
        bool insertNode(int key, int value) {
            struct Node *pre[max_level]; 
            struct Node *curr = head; 

            // 寻找前驱节点             
            for ( int i = max_level - 1 ; i >= 0 ; i-- ) {
                if ( curr->next[i] == tail ) {
                    pre[i] = curr; 
                }
                else {
                    while ( curr->next[i] != tail && curr->next[i]->key < key ) {
                        curr = curr->next[i];  
                    }
                    // 节点已经存在 
                    if ( curr->next[i] != tail && curr->next[i]->key == key ) {
                        curr->next[i]->value = value; 
                        return true; 
                    }
                    pre[i] = curr; 
                }
            }
            
            // 构造节点 
            int level = RandomLevel(); 
            struct Node* tmp = new struct Node;
            tmp->key = key;
            tmp->value = value; 
            tmp->level = level; 
            tmp->next = new struct Node *[level+1]; 

            // 插入节点 
            for ( int i = 0 ; i <= level ; i++ ) {
                tmp->next[i] = pre[i]->next[i];  
                pre[i]->next[i] = tmp; 
            }

            return true;        
        }

        // 删除节点 
        bool deleteNode(int key) {
            struct Node *pre[max_level];   
            struct Node *curr = head; 

            // 寻找前驱节点 
            for ( int i = max_level - 1 ; i >= 0 ; i-- ) {
                if ( curr->next[i] == tail ) {
                    pre[i] = NULL;  
                }
                else {
                    while ( curr->next[i] != tail && curr->next[i]->key < key ) {
                        curr = curr->next[i]; 
                    }
                    if ( curr->next[i] != tail && curr->next[i]->key == key ) {
                        pre[i] = curr; 
                    } 
                    else {
                        pre[i] = NULL;
                    }
                }
            }            
            
            // 如果在最底层都找不到这个节点的话，说明节点不存在 
            if ( pre[0] == NULL ) {
                printf("节点不存在！\n"); 
                return false; 
            }

            // 删除节点 
            struct Node *tmp = NULL; 
            for ( int i = 0 ; i < max_level ; i++ ) {
                if ( pre[i] == NULL ) {
                    delete tmp->next; 
                    delete tmp; 
                    break;
                } 
                tmp = pre[i]->next[i]; 
                pre[i]->next[i] = tmp->next[i];  
            }

            return true; 
        }
         
        // return Node  
        // 查找
        struct Node* getNode(int key) {
            struct Node *curr = head; 
            
            for ( int i = max_level - 1 ; i >= 0 ; i-- ) {
                if ( curr->next[i] != tail ) {
                    while ( curr->next[i] != tail && curr->next[i]->key < key ) {
                        curr = curr->next[i]; 
                    }
                    if ( curr->next[i] != tail && curr->next[i]->key == key ) {
                        return curr->next[i]; 
                    } 
                }
            }
            return NULL; 
        }
        
        void printSkipList() {
            for ( int i = max_level - 1 ; i >= 0 ; i-- ) {
                printf("level: %d  ",i);
                struct Node *curr = head->next[i];
                struct Node *base = head->next[0]; 
                while ( base != tail ) {
                    if ( curr == base ) {
                        printf("(%d, %d) ",curr->key,curr->value); 
                        curr = curr->next[i]; 
                    }
                    else {
                        printf("       "); 
                    }
                    base = base->next[0]; 
                }
                printf("\n"); 
            } 
            printf("\n\n"); 
        }

        int RandomLevel() {
            int level = 0;
            while ( rand() % 2 && level < max_level - 1 ) {
                level++; 
            }
            return level; 
        }
}; 


void test() {
    SkipList sl = SkipList(3); 
    for ( int i = 0 ; i < 10 ; i++ ) {
        bool res = sl.insertNode(i,i);
        sl.printSkipList(); 
    }
    sl.insertNode(0,2);
    sl.printSkipList(); 

    sl.insertNode(1,2);
    sl.printSkipList(); 
    
    for ( int i = 12 ; i >= 0 ; i-- ) {
        bool res = sl.deleteNode(i);
        printf("res: %d \n",res); 
        sl.printSkipList(); 
    }
}


int main() {
    test();
    return 0; 
}