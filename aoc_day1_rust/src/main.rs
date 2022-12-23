use std::fs::File;
use std::io::prelude::*;

use partial_sort::PartialSort;

fn main() -> std::io::Result<()> {
    let mut file = File::open("input.txt")?;
    let mut contents = String::new();
    file.read_to_string(&mut contents)?;

    let mut calories_per_elf : Vec<i32> = Vec::new();
    let mut calories_on_elf = 0;

    for line in contents.lines() {
        if line.is_empty(){
            calories_per_elf.push(calories_on_elf);
            calories_on_elf = 0;
            continue;
        }
        calories_on_elf += line.parse::<i32>().expect("line is not a valid number");
    }

    let elfs_to_sort = 3;
    calories_per_elf.partial_sort(elfs_to_sort, |a,b| b.cmp(a));

    let total_callories : i32 = calories_per_elf[0.. elfs_to_sort].iter().sum();
    print!("{}", total_callories);

    Ok(())
}