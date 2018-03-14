//// [helloworld.jump]
const Log = {
  info(msg: string) {}
}

class HelloWorld {
  constructor(private name: string) {
  }

  public hello() {
    Log.info(`Hello ${this.name}`);
  }
}




//// [helloworld.d.jump]
declare const Log: {
    info(msg: string): void;
};
declare class HelloWorld {
    private name;
    constructor(name: string);
    hello(): void;
}
