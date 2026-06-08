const fs = require("fs");

const config = JSON.parse(fs.readFileSync("../flow.cppflowproj", "utf8"));
process.stdout.write(config.name);