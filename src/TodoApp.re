type item = {
  title: string,
  completed: bool,
  id: int
};

type state = {
  /* type with a type argument equivalent to List<Item> */
  items: list(item)
};

type action =
  | AddItem
  | ToggleItem(int);

let component = ReasonReact.reducerComponent("TodoApp");

let lastId = ref(0);

let newItem = () => {
  lastId := lastId^ + 1;
  {id: lastId^, title: "Click a button", completed: true}
};

let str = ReasonReact.stringToElement;

module TodoItem = {
  let component = ReasonReact.statelessComponent("TodoItem");
  let make = (~item, ~onToggle, children) => {
    ...component,
    render: (_) =>
      <div className="item" onClick=((_evt) => onToggle())>
        <input _type="checkbox" checked=(Js.Boolean.to_js_boolean(item.completed)) />
        (str(item.title))
      </div>
  };
};

let make = (children) => {
  ...component,
  initialState: () => {items: [{title: "Write some things to do", completed: false, id: 0}]},
  reducer: (action, {items}) =>
    switch action {
    | AddItem => ReasonReact.Update({items: [newItem(), ...items]})
    | ToggleItem(id) =>
      let items =
        List.map((item) => item.id === id ? {...item, completed: ! item.completed} : item, items);
      ReasonReact.Update({items: items})
    },
  render: ({state: {items}, reduce}) => {
    let numItems = List.length(items);
    <div className="app">
      <div className="title"> (str("What to do")) </div>
      <button onClick=(reduce((_evt) => AddItem))> (str("Add something")) </button>
      <div className="items">
        (
          List.map(
            (item) =>
              <TodoItem
                key=(string_of_int(item.id))
                onToggle=(reduce(() => ToggleItem(item.id)))
                item
              />,
            items
          )
          |> Array.of_list
          |> ReasonReact.arrayToElement
        )
      </div>
      <footer> (str(string_of_int(numItems) ++ " items")) </footer>
    </div>
  }
};
