import { IsArray, IsIn } from 'class-validator';

export class GenerateFirmwareDto {
  @IsArray()
  @IsIn(['weather', 'soil', 'npk', 'gps'], { each: true })
  sensors!: string[];
}
