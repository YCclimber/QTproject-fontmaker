#ifndef LIST
#define LIST


struct list_head{
    struct list_head *next,*prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head *list)
{
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head *new_m,
                  struct list_head *prev,
                  struct list_head *next)
{
    next->prev = new_m;
    new_m->next = next;
    new_m->prev = prev;
    prev->next = new_m;
}

static inline void list_add(struct list_head *new_m, struct list_head *head)
{
    __list_add(new_m, head, head->next);
}

static inline void list_add_tail(struct list_head *new_m, struct list_head *head)
{
    __list_add(new_m, head->prev, head);
}

static inline void __list_del(struct list_head * prev, struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

static inline int list_is_last(const struct list_head *list,
                const struct list_head *head)
{
    return list->next == head;
}

static inline int list_empty(const struct list_head *head)
{
    return head->next == head;
}

#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
        pos = n, n = pos->next)

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#endif // LIST

