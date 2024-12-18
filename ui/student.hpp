#include <string>

namespace ui {
    std::string StudentHTML = R"HTML(
<html>

<head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <link href="https://unpkg.com/layui@2.9.20/dist/css/layui.css" rel="stylesheet">
</head>

<body>
    <script src="https://unpkg.com/vue@3/dist/vue.global.js"></script>
    <script src="https://unpkg.com/layui@2.9.20/dist/layui.js"></script>
    <div id="studentInfoDialog" style="display: none; padding: 20px;">
        <form class="layui-form" action="">
            <div class="layui-form-item">
                <label class="layui-form-label">姓名</label>
                <div class="layui-input-block">
                    <input type="text" name="name" required lay-verify="required" placeholder="请输入姓名" autocomplete="off"
                        class="layui-input">
                </div>
            </div>
            <div class="layui-form-item">
                <label class="layui-form-label">性别</label>
                <div class="layui-input-block">
                    <input type="radio" name="gender" value="男" title="男" checked>
                    <input type="radio" name="gender" value="女" title="女">
                </div>
            </div>
        </form>
    </div>
    <div id="app">
        <div style="margin: 15px;">
            <button class="layui-btn" onclick="window.changeToIndex()">返回主页</button>
            <br>
            <br>
            <div class="layui-form">
                <select lay-filter="class-select-filter">
                    <option value="">请选择</option>
                    <option v-for="i in classList" :value="i" :key="i">{{ i }}</option>
                </select>
            </div>
            <br>
            <button class="layui-btn" @click="loadData">加载</button>
            <button class="layui-btn layui-btn-normal" @click="addClass">新建班级</button>
            <button class="layui-btn layui-btn-danger" @click="deleteClass">删除班级</button>
            <br>
            <br>
            <button class="layui-btn layui-btn-normal" @click="addStudent">增加学生</button>
            <br>
            <br>
            <div v-if="studentsList.length > 0">
                <table class="layui-table">
                    <thead>
                        <tr>
                            <th>序号</th>
                            <th>姓名</th>
                            <th>性别</th>
                            <th>操作</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr v-for="i in studentsList">
                            <td>{{i.id}}</td>
                            <td>{{i.name}}</td>
                            <td>{{i.sex}}</td>
                            <td>
                                <button class="layui-btn layui-btn-danger" @click="deleteStudent(i.name)">删除</button>
                            </td>
                        </tr>
                    </tbody>
                </table>
            </div>
        </div>
    </div>

    <script>
        let $ = layui.$;
        let selectClass = "";
        const { createApp } = Vue

        createApp({
            data() {
                return {
                    classList: [],
                    studentsList: []
                }
            },
            methods: {
                async loadData() {
                    if (selectClass == "") {
                        layer.msg("请先选中或者创建一个班级");
                        return;
                    }
                    this.studentsList = (await window.getStudents(selectClass)).data;
                    console.log(this.studentsList);
                },
                addClass() {
                    var that = this;
                    layer.prompt({ title: '请输入要添加的班级名（例如软国2407）：' }, async function (value, index, elem) {
                        if (value === '') return elem.focus();
                        await window.addClass(value);
                        layer.msg("添加成功！");
                        that.loadClass();
                        that.studentsList = [];
                        layer.close(index);
                    });
                },
                async loadClass() {
                    var i = await window.getClass();
                    this.classList = i.data;
                    console.info(this.classList);
                    this.$nextTick(() => {
                        layui.form.render('select');
                    });
                },
                async deleteClass() {
                    if (selectClass == "") {
                        layer.msg("请至少选择一个班级");
                        return;
                    }
                    await window.removeClass(selectClass);
                    this.loadClass();
                    selectClass = "";
                },
                addStudent() {
                    if (selectClass == "") {
                        layer.msg("请选择或新建一个班级！");
                        return;
                    }
                    var that = this;
                    layui.use(['layer', 'form'], function () {
                        var layer = layui.layer;
                        var form = layui.form;
                        var thatThis = that;
                        layer.open({
                            type: 1,
                            title: '输入学生信息',
                            content: $('#studentInfoDialog'),
                            area: ['350px', '250px'],
                            btn: ['确定', '取消'],
                            yes: function (index, layero) {
                                var name = layero.find('input[name=name]').val();
                                var gender = layero.find('input[name=gender]:checked').val();
                                if (!name) {
                                    layer.msg('请输入姓名');
                                    return;
                                }
                                window.addStudent(selectClass, name, gender);
                                thatThis.loadData();
                                layer.close(index);
                            }
                        });

                    });
                },
                async deleteStudent(name) {
                    await window.deleteStudent(selectClass,name);
                    this.loadData();
                }
            },
            mounted() {
                this.loadClass();
            }
        }).mount('#app');

        layui.use(function () {

            var form = layui.form;
            var layer = layui.layer;
            form.on('select(class-select-filter)', function (data) {
                var value = data.value;
                selectClass = value;
            });
        });
    </script>
</body>

</html>
)HTML";
}