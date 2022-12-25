use std::{fs::read_to_string, mem::swap};
use sscanf::scanf;
struct Range {
    start : i32, 
    end : i32
}
fn main() {
    let content = read_to_string("input.txt").unwrap();

    let mut number_of_contained_ranges = 0;
    for line in content.lines(){
        
        let parsed = scanf!(line, "{}-{},{}-{}", i32, i32, i32, i32);
        let (start, end, start_2, end_2) = parsed.unwrap();
        
        let mut elf_one_range  = Range { start: start, end: end };
        let mut elf_two_range  = Range { start: start_2, end: end_2 };
        
        if elf_two_range.start < elf_one_range.start {
            swap(&mut elf_one_range, &mut elf_two_range);
        } else if elf_two_range.start == elf_one_range.start && elf_one_range.end < elf_two_range.end {
            swap(&mut elf_one_range, &mut elf_two_range)
        }

        if elf_one_range.start <= elf_two_range.end && elf_two_range.start <= elf_one_range.end {
			number_of_contained_ranges += 1;
        }
    }

    print!("{number_of_contained_ranges}");
}
