use std::fs::read_to_string;
use std::collections::{HashMap, HashSet};
fn main() {
    let content = read_to_string("input.txt").unwrap();
    let mut total_sum = 0;
    let mut item_count_map : HashMap<char, i32> = HashMap::new();

    for (line_number, line) in content.lines().enumerate(){
        let unique_items : HashSet<_> = line.chars().collect();
        for item in unique_items{
            *item_count_map.entry(item).or_insert(0) += 1;
        }

        if (line_number + 1) % 3 == 0 {
            let item = item_count_map.iter().find_map(|(&key, &value)| if value == 3 { Some(key)} else { None }).unwrap();
            total_sum += (if item.is_uppercase() { item  as i32 - 'A' as i32 + 26 } else { item as i32 - 'a' as i32 }) + 1;
            item_count_map.clear();
       }
    }

    print!("{total_sum}");
}
