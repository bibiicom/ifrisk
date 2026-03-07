import {sequelize} from "../config/database";


async function run() {

    await sequelize.close();
}

run();
