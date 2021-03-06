package LinkList;

//class of node
class Node {
	private Node next;
	private int value;

	Node(int val) {
		value = val;
	}

	public int getValue() {
		return value;
	}

	public Node getNext() {
		return next;
	}

	public void setValue(int val) {
		value = val;
	}

	public void setNext(Node nex) {
		next = nex;
	}
}

class MyLinkList {
	private Node head;
	private Node tail;
	private int size = 0;

	// add nodes
	public void append(int newData) {
		Node newNode = new Node(newData);
		size++;
		if (head == null) {
			head = newNode;
			tail = newNode;
			return;
		}
		tail.setNext(newNode);
		tail = newNode;
	}

	// insert nodes
	public void insert(int position, int newData) {
		Node node = head;
		Node newNode = new Node(newData);
		if (position == 0) {
			newNode.setNext(node);
			head = newNode;
		} else {
			for (int i = 0; i < position - 1; i++) {
				node = node.getNext();
			}
			Node nodeNext = node.getNext();
			node.setNext(newNode);
			newNode.setNext(nodeNext);
		}
		size++;
	}

	// get the length
	public int getSize() {
		return size;
	}

	// search
	public int getNodeData(int position) {
		if (position < 0 || position >= size) {
			return -1;
		}
		Node node = head;
		for (int i = 0; i < position; i++) {
			node = node.getNext();
		}
		int data = node.getValue();
		return data;
	}

	// delete
	public void delete(int position) {
		Node node = head;
		if (position == 0) {
			node = node.getNext();
			head = node;
		} else {
			for (int i = 0; i < position - 1; i++) {
				node = node.getNext();
			}
			Node nextNode = node.getNext();
			node.setNext(nextNode.getNext());
		}
		size -= 1;
	}

	// print
	public void show() {
		Node node = head;
		int value;
		for (int i = 0; i < size - 1; i++) {
			value = node.getValue();
			System.out.print(value);
			System.out.print("->");
			node = node.getNext();
		}
		System.out.print(node.getValue());
		System.out.print("\n");
	}
}

// test
public class LinkList {
	public static void main(String[] args) {
		MyLinkList link = new MyLinkList();
		for (int i = 0; i < 20; i += 2)
			link.append(i);
		link.show();
		link.insert(1, 5);
		link.show();
		System.out.println(link.getSize());
		link.delete(2);
		link.show();
		System.out.println(link.getSize());
		System.out.println(link.getNodeData(3));
	}

}
