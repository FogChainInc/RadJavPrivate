"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
class Block {
    constructor(start, end) {
        this.start = start;
        this.end = end;
        this.keywordStart = this.start + `/*^:`.length;
        this.contents = "";
    }
    static findBlock(content) {
        let start = content.indexOf(`/*^:`);
        let end = content.indexOf("*/", start);
        let block = null;
        if (start > -1) {
            block = new Block(start, end);
            block.contents = content.substring(block.keywordStart, (end - 1));
        }
        return (block);
    }
}
exports.Block = Block;
//# sourceMappingURL=Block.js.map