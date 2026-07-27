// This  just currently works for infix operators
// TODO make it work for all

enum Node {
    Operand(char),
    Operator {
        op: char,
        priority: u8,
        left: Box<Node>,
        right: Box<Node>,
    },
}
impl Node {
    // fxn to create operand leaf node.
    fn operand(val: char) -> Self {
        Node::Operand(val)
    }
    fn insert_operator(self, op, priority, right_operand:Node)->Self {
        match self {

        }
    }
}
fn main() {}
