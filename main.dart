import 'package:flutter/material.dart';

void main() {
  runApp(TodoApp());
}

class TodoApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'To-Do List',
      theme: ThemeData(
        primarySwatch: Colors.blue,
        visualDensity: VisualDensity.adaptivePlatformDensity,
      ),
      home: TodoListScreen(),
      debugShowCheckedModeBanner: false,
    );
  }
}

class Todo {
  int id;
  String title;
  String description;
  bool isCompleted;
  DateTime createdAt;

  Todo({
    required this.id,
    required this.title,
    this.description = '',
    this.isCompleted = false,
    required this.createdAt,
  });
}

class TodoListScreen extends StatefulWidget {
  @override
  _TodoListScreenState createState() => _TodoListScreenState();
}

class _TodoListScreenState extends State<TodoListScreen> {
  List<Todo> todos = [];
  int nextId = 1;
  String filter = 'all'; // all, completed, pending

  final TextEditingController titleController = TextEditingController();
  final TextEditingController descriptionController = TextEditingController();

  @override
  void initState() {
    super.initState();
    // 샘플 데이터 추가
    _addSampleData();
  }

  void _addSampleData() {
    todos.addAll([
      Todo(
        id: nextId++,
        title: '프로젝트 기획서 작성',
        description: '새로운 앱 개발을 위한 기획서를 완성해야 합니다.',
        createdAt: DateTime.now().subtract(Duration(days: 1)),
      ),
      Todo(
        id: nextId++,
        title: '운동하기',
        description: '헬스장에서 1시간 운동',
        createdAt: DateTime.now().subtract(Duration(hours: 2)),
        isCompleted: true,
      ),
      Todo(
        id: nextId++,
        title: '책 읽기',
        description: 'Flutter 개발서 30페이지 읽기',
        createdAt: DateTime.now(),
      ),
    ]);
  }

  void _addTodo() {
    if (titleController.text.isNotEmpty) {
      setState(() {
        todos.add(Todo(
          id: nextId++,
          title: titleController.text,
          description: descriptionController.text,
          createdAt: DateTime.now(),
        ));
      });
      titleController.clear();
      descriptionController.clear();
      Navigator.of(context).pop();
    }
  }

  void _toggleTodo(int id) {
    setState(() {
      final todo = todos.firstWhere((todo) => todo.id == id);
      todo.isCompleted = !todo.isCompleted;
    });
  }

  void _deleteTodo(int id) {
    setState(() {
      todos.removeWhere((todo) => todo.id == id);
    });
  }

  void _editTodo(Todo todo) {
    titleController.text = todo.title;
    descriptionController.text = todo.description;
    
    _showTodoDialog(
      title: '할 일 수정',
      onSave: () {
        if (titleController.text.isNotEmpty) {
          setState(() {
            todo.title = titleController.text;
            todo.description = descriptionController.text;
          });
          titleController.clear();
          descriptionController.clear();
          Navigator.of(context).pop();
        }
      },
    );
  }

  List<Todo> get filteredTodos {
    switch (filter) {
      case 'completed':
        return todos.where((todo) => todo.isCompleted).toList();
      case 'pending':
        return todos.where((todo) => !todo.isCompleted).toList();
      default:
        return todos;
    }
  }

  void _showTodoDialog({String title = '새 할 일 추가', required VoidCallback onSave}) {
    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        title: Text(title),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            TextField(
              controller: titleController,
              decoration: InputDecoration(
                labelText: '제목',
                border: OutlineInputBorder(),
              ),
              autofocus: true,
            ),
            SizedBox(height: 16),
            TextField(
              controller: descriptionController,
              decoration: InputDecoration(
                labelText: '설명 (선택사항)',
                border: OutlineInputBorder(),
              ),
              maxLines: 3,
            ),
          ],
        ),
        actions: [
          TextButton(
            onPressed: () {
              titleController.clear();
              descriptionController.clear();
              Navigator.of(context).pop();
            },
            child: Text('취소'),
          ),
          ElevatedButton(
            onPressed: onSave,
            child: Text('저장'),
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    final filteredList = filteredTodos;
    final completedCount = todos.where((todo) => todo.isCompleted).length;
    final totalCount = todos.length;

    return Scaffold(
      appBar: AppBar(
        title: Text('할 일 목록'),
        backgroundColor: Colors.blue,
        foregroundColor: Colors.white,
        actions: [
          PopupMenuButton<String>(
            icon: Icon(Icons.filter_list),
            onSelected: (value) {
              setState(() {
                filter = value;
              });
            },
            itemBuilder: (context) => [
              PopupMenuItem(value: 'all', child: Text('전체')),
              PopupMenuItem(value: 'pending', child: Text('진행중')),
              PopupMenuItem(value: 'completed', child: Text('완료됨')),
            ],
          ),
        ],
      ),
      body: Column(
        children: [
          // 진행 상황 표시
          Container(
            width: double.infinity,
            padding: EdgeInsets.all(16),
            color: Colors.blue.shade50,
            child: Column(
              children: [
                Text(
                  '진행 상황',
                  style: TextStyle(
                    fontSize: 18,
                    fontWeight: FontWeight.bold,
                    color: Colors.blue.shade800,
                  ),
                ),
                SizedBox(height: 8),
                LinearProgressIndicator(
                  value: totalCount > 0 ? completedCount / totalCount : 0,
                  backgroundColor: Colors.grey.shade300,
                  valueColor: AlwaysStoppedAnimation<Color>(Colors.blue),
                ),
                SizedBox(height: 8),
                Text(
                  '$completedCount / $totalCount 완료',
                  style: TextStyle(
                    color: Colors.blue.shade700,
                    fontWeight: FontWeight.w500,
                  ),
                ),
              ],
            ),
          ),
          
          // 필터 상태 표시
          if (filter != 'all')
            Container(
              width: double.infinity,
              padding: EdgeInsets.symmetric(horizontal: 16, vertical: 8),
              color: Colors.orange.shade100,
              child: Row(
                children: [
                  Icon(Icons.filter_list, size: 16, color: Colors.orange.shade700),
                  SizedBox(width: 8),
                  Text(
                    '필터: ${filter == 'completed' ? '완료된 항목' : '진행중인 항목'}',
                    style: TextStyle(color: Colors.orange.shade700),
                  ),
                  Spacer(),
                  GestureDetector(
                    onTap: () => setState(() => filter = 'all'),
                    child: Text(
                      '전체 보기',
                      style: TextStyle(
                        color: Colors.orange.shade700,
                        decoration: TextDecoration.underline,
                      ),
                    ),
                  ),
                ],
              ),
            ),

          // 할 일 목록
          Expanded(
            child: filteredList.isEmpty
                ? Center(
                    child: Column(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        Icon(
                          filter == 'completed' ? Icons.task_alt : Icons.inbox,
                          size: 64,
                          color: Colors.grey,
                        ),
                        SizedBox(height: 16),
                        Text(
                          filter == 'completed' 
                              ? '완료된 할 일이 없습니다'
                              : filter == 'pending'
                                  ? '진행중인 할 일이 없습니다'
                                  : '할 일을 추가해보세요!',
                          style: TextStyle(
                            fontSize: 18,
                            color: Colors.grey.shade600,
                          ),
                        ),
                      ],
                    ),
                  )
                : ListView.builder(
                    itemCount: filteredList.length,
                    padding: EdgeInsets.all(16),
                    itemBuilder: (context, index) {
                      final todo = filteredList[index];
                      return Card(
                        margin: EdgeInsets.only(bottom: 8),
                        elevation: 2,
                        child: ListTile(
                          leading: Checkbox(
                            value: todo.isCompleted,
                            onChanged: (_) => _toggleTodo(todo.id),
                          ),
                          title: Text(
                            todo.title,
                            style: TextStyle(
                              decoration: todo.isCompleted
                                  ? TextDecoration.lineThrough
                                  : null,
                              color: todo.isCompleted
                                  ? Colors.grey
                                  : null,
                            ),
                          ),
                          subtitle: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              if (todo.description.isNotEmpty)
                                Text(
                                  todo.description,
                                  style: TextStyle(
                                    color: todo.isCompleted
                                        ? Colors.grey
                                        : Colors.grey.shade600,
                                  ),
                                ),
                              SizedBox(height: 4),
                              Text(
                                _formatDate(todo.createdAt),
                                style: TextStyle(
                                  fontSize: 12,
                                  color: Colors.grey.shade500,
                                ),
                              ),
                            ],
                          ),
                          isThreeLine: todo.description.isNotEmpty,
                          trailing: Row(
                            mainAxisSize: MainAxisSize.min,
                            children: [
                              IconButton(
                                icon: Icon(Icons.edit, color: Colors.blue),
                                onPressed: () => _editTodo(todo),
                              ),
                              IconButton(
                                icon: Icon(Icons.delete, color: Colors.red),
                                onPressed: () => _deleteTodo(todo.id),
                              ),
                            ],
                          ),
                        ),
                      );
                    },
                  ),
          ),
        ],
      ),
      floatingActionButton: FloatingActionButton(
        onPressed: () => _showTodoDialog(onSave: _addTodo),
        child: Icon(Icons.add),
        backgroundColor: Colors.blue,
        foregroundColor: Colors.white,
      ),
    );
  }

  String _formatDate(DateTime date) {
    final now = DateTime.now();
    final today = DateTime(now.year, now.month, now.day);
    final todoDate = DateTime(date.year, date.month, date.day);

    if (todoDate == today) {
      return '오늘 ${date.hour}:${date.minute.toString().padLeft(2, '0')}';
    } else if (todoDate == today.subtract(Duration(days: 1))) {
      return '어제 ${date.hour}:${date.minute.toString().padLeft(2, '0')}';
    } else {
      return '${date.month}/${date.day} ${date.hour}:${date.minute.toString().padLeft(2, '0')}';
    }
  }

  @override
  void dispose() {
    titleController.dispose();
    descriptionController.dispose();
    super.dispose();
  }
}
