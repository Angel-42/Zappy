### 🌟 GOAL: SURVIVE

 **Condition** : Food < 5

 **Actions** :

* `Inventory`
* `Look`
* Search tile vision for `food`
* Move towards food
* `Take food`
* If no food → `Forward` and repeat

---

### 🌟 GOAL: LEVEL UP

 **Condition** : Has needed stones + enough same-level players

 **Actions** :

* `Inventory`
* `Look`
* `Broadcast "RITUAL at (X,Y) level L"`
* Wait for allies
* `Set linemate`, `Set sibur`, ...
* `Incantation`
* Check for success

> Use a table to match current level to elevation requirements (from the PDF).

---

### 🌟 GOAL: REPRODUCE

 **Condition** : `connect_nbr` > 0 and inventory has enough food

 **Actions** :

* `Fork`

---

### 🌟 GOAL: EXPLORE

 **Condition** : Idle or nothing better to do

 **Actions** :

* Random movement
* `Look` and react to surroundings
