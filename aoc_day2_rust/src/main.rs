use std::fs::File;
use std::io::prelude::*;

static LOOSELOOKUPTABLE: [usize; 3] = [2, 0 ,1];
static WINLOOKUPTABLE: [usize; 3] = [1,2,0];
fn main() -> std::io::Result<()>{
    println!("Hello, world!");
    let mut file = File::open("input.txt")?;
    let mut contents = String::new();
    file.read_to_string(&mut contents)?;

    let mut total_score : usize = 0;
    for line in contents.lines(){
        assert!(line.len() == 3, "unexpedted format");
        
        let opponent_choice = line.chars().nth(0).unwrap() as usize - 'A' as usize;
        let desired_game_result = line.chars().nth(2).unwrap() as usize - 'X' as usize;

        if desired_game_result == 0{
            total_score += LOOSELOOKUPTABLE[opponent_choice] + 1;
        }
        else if desired_game_result == 1{
            total_score += opponent_choice + 1 + 3;
        }
        else {
            total_score += WINLOOKUPTABLE[opponent_choice] + 1 + 6;
        }
    }

    print!("{total_score}");

    Ok(())
}
