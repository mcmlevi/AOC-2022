use std::{fs::read_to_string, collections::VecDeque, str::Lines, ops::Range};
use sscanf::sscanf;

type ContainerList = Vec<VecDeque<char>>;

fn get_containers(content : &mut Lines) -> ContainerList {

    let mut containers = Vec::<VecDeque<char>>::new();

    for line in content.into_iter() {

        if line.is_empty() {
            break;
        }

        let mut container_index = 0;
        let range = Range { start : 0, end : line.len() - 1};
        for index in range.step_by(4)
        {
            let cargo : char = line.chars().nth(index + 1).unwrap();
            let is_valid_cargo = if cargo != ' ' && !cargo.is_digit(10) { true } else { false };
            
            if is_valid_cargo {
                if containers.len() <= container_index {
                    containers.resize(container_index + 1, VecDeque::new());
                }
                containers[container_index].push_front(cargo);
            }
            container_index += 1;
        }
   }
   containers
}

struct Instuction
{
    count :usize,
    source : usize,
    destination :usize,
}

fn get_instruction(line : &str) -> Instuction {
    let (count, source, destination) = sscanf!(line, "move {} from {} to {}", usize, usize, usize).unwrap();
    Instuction { count: count, source: source - 1, destination: destination - 1 }
}

fn main() {
    let content = read_to_string("input.txt").unwrap();
    let mut it = content.lines();

    let mut container_list = get_containers(&mut it);

    for line in it.into_iter() {
        let instruction = get_instruction(line);
        let mut stack_to_move = VecDeque::<char>::new();
        
        for _ in 0..instruction.count {
            let source_container  = &mut container_list[instruction.source];
            let item = source_container.pop_back().unwrap();
            stack_to_move.push_front(item);
        }

        for cargo_item in stack_to_move {
            container_list[instruction.destination].push_back(cargo_item);
        }
    }

    let mut top_cargo_list = String::new();
    for container in container_list {
        top_cargo_list.push(container.back().unwrap().clone());
    }

    print!("{top_cargo_list}");
}
