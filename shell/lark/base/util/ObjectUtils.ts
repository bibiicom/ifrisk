import {BaseClass} from "../core/BaseClass";

export class ObjectUtils extends BaseClass {
    public object2array(object: Object): any[] {
        let res = [];
        for (let key in object) {
            if (object.hasOwnProperty(key)) {
                let element = object[key];
                res.push(element);
            }
        }
        return res
    }

    public parseData(self: any, data: any): void {
        for (let key in data) {
            if (this.hasOwnProperty(key)) {
                switch (data[key]) {
                    case "true":
                        self[key] = true;
                        break;
                    case "TRUE":
                        self[key] = true;
                        break;
                    case "false":
                        self[key] = false;
                        break;
                    case "FALSE":
                        self[key] = false;
                        break;
                    default:
                        self[key] = data[key];
                        break;
                }
            }
        }
    }

    public strRender(template: string, map: any): string {
        if (!template) {
            return '';
        }
        let reg = /{{(.*?)}}/g;
        return template.replace(reg, (item, key) => {
            return map[key] ? map[key] : '';
        });
    }

    public removeDuplicates(arr: number[]): number[] {
        return arr.filter((item, index) => arr.indexOf(item) === index);
    }

    public binarySearch(arr: any[], target: number): number {
        let left: number = 0;
        let right: number = arr.length - 1;

        while (left <= right) {
            const mid: number = Math.floor((left + right) / 2); // 计算中间索引
            const midValue: number = arr[mid];

            if (midValue === target) {
                // 找到目标值，返回索引
                return mid;
            } else if (midValue < target) {
                // 如果目标值大于中间值，搜索右半部分
                left = mid + 1;
            } else {
                // 如果目标值小于中间值，搜索左半部分
                right = mid - 1;
            }
        }

        // 如果没有找到目标值，返回-1
        return -1;
    }

    public equalsObjFast(obj1: any, obj2: any): boolean {
        let i = 0;
        let isSame = true;
        for (let key in obj1) {
            if (obj1[key] != obj2[key]) {
                isSame = false;
                break;
            }
            i++;
            if (i > 10) {
                break;
            }
        }
        return isSame;
    }


}
